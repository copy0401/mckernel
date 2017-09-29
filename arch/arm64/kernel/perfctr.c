/* perfctr.c COPYRIGHT FUJITSU LIMITED 2015-2017 */
#include <arch-perfctr.h>
#include <ihk/perfctr.h>
#include <mc_perf_event.h>
#include <errno.h>
#include <ihk/debug.h>
#include <registers.h>
#include <string.h>

/*
 * @ref.impl arch/arm64/kernel/perf_event.c
 * Set at runtime when we know what CPU type we are.
 */
struct arm_pmu cpu_pmu;
extern int ihk_param_pmu_irq_affiniry[CONFIG_SMP_MAX_CORES];
extern int ihk_param_nr_pmu_irq_affiniry;


int arm64_init_perfctr(void)
{
	int ret;
	int i;

	memset(&cpu_pmu, 0, sizeof(cpu_pmu));
	ret = armv8pmu_init(&cpu_pmu);
	if (!ret) {
		return ret;
	}
	for (i = 0; i < ihk_param_nr_pmu_irq_affiniry; i++) {
		ret = ihk_mc_register_interrupt_handler(ihk_param_pmu_irq_affiniry[i], cpu_pmu.handler);
	}
	return ret;
}

int arm64_enable_pmu(void)
{
	int ret;
	if (cpu_pmu.reset) {
		cpu_pmu.reset(&cpu_pmu);
	}
	ret = cpu_pmu.enable_pmu();
	return ret;
}

void arm64_disable_pmu(void)
{
	cpu_pmu.disable_pmu();
}

int arm64_enable_user_access_pmu_regs(void)
{
	int ret;
	ret = cpu_pmu.enable_user_access_pmu_regs();
	return ret;
}

int arm64_disable_user_access_pmu_regs(void)
{
	int ret;
	ret = cpu_pmu.disable_user_access_pmu_regs();
	return ret;
}

extern unsigned int *arm64_march_perfmap;

static int __ihk_mc_perfctr_init(int counter, uint32_t type, uint64_t config, int mode)
{
	int ret;
	unsigned long config_base = 0;
	int mapping;

	mapping = cpu_pmu.map_event(type, config);
	if (mapping < 0) {
		return mapping;
	}

	ret = cpu_pmu.disable_counter(counter);
	if (!ret) {
		return ret;
	}

	ret = cpu_pmu.enable_intens(counter);
	if (!ret) {
		return ret;
	}

	ret = cpu_pmu.set_event_filter(&config_base, mode);
	if (!ret) {
		return ret;
	}
	config_base |= (unsigned long)mapping;
	cpu_pmu.write_evtype(counter, config_base);
	return ret;
}

int ihk_mc_perfctr_init_raw(int counter, uint64_t config, int mode)
{
	int ret;
	ret = __ihk_mc_perfctr_init(counter, PERF_TYPE_RAW, config, mode);
	return ret;
}

int ihk_mc_perfctr_init(int counter, uint64_t config, int mode)
{
	int ret;
	ret = __ihk_mc_perfctr_init(counter, PERF_TYPE_RAW, config, mode);
	return ret;
}

int ihk_mc_perfctr_start(unsigned long counter_mask)
{
	int ret = 0, i;

	for (i = 0; i < sizeof(counter_mask) * BITS_PER_BYTE; i++) {
		if (counter_mask & (1UL << i)) {
			ret = cpu_pmu.enable_counter(i);
			if (ret) {
				kprintf("%s: enable failed(idx=%d)\n", i);
				break;
			}
		}
	}
	return ret;
}

int ihk_mc_perfctr_stop(unsigned long counter_mask)
{
	int i = 0;

	for (i = 0; i < sizeof(counter_mask) * BITS_PER_BYTE; i++) {
		if (counter_mask & (1UL << i)) {
			cpu_pmu.disable_counter(i);

			// ihk_mc_perfctr_startが呼ばれるときには、
			// init系関数が呼ばれるのでdisableにする。
			cpu_pmu.disable_intens(i);
		}
	}
	return 0;
}

int ihk_mc_perfctr_reset(int counter)
{
	// TODO[PMU]: ihk_mc_perfctr_setと同様にサンプリングレートの共通部実装の扱いを見てから本実装。
	cpu_pmu.write_counter(counter, 0);
	return 0;
}

int ihk_mc_perfctr_set(int counter, long val)
{
	// TODO[PMU]: 共通部でサンプリングレートの計算をして、設定するカウンタ値をvalに渡してくるようになると想定。サンプリングレートの扱いを見てから本実装。
	uint32_t v = val;
	cpu_pmu.write_counter(counter, v);
	return 0;
}

int ihk_mc_perfctr_read_mask(unsigned long counter_mask, unsigned long *value)
{
	/* this function not used yet. */
	panic("not implemented.");
	return 0;
}

unsigned long ihk_mc_perfctr_read(int counter)
{
	unsigned long count;
	count = cpu_pmu.read_counter(counter);
	return count;
}

int ihk_mc_perfctr_alloc_counter(unsigned int *type, unsigned long *config, unsigned long pmc_status)
{
	int ret;

	if(*type == PERF_TYPE_HARDWARE) {
		switch(*config){
		case PERF_COUNT_HW_INSTRUCTIONS :
			*config = cpu_pmu.map_event(*type, *config);
			*type = PERF_TYPE_RAW;
			break;
		default :
			// Unexpected config
			return -1;
		}
	}
	else if(*type != PERF_TYPE_RAW) {
		return -1;
	}
	ret = cpu_pmu.get_event_idx(cpu_pmu.num_events, pmc_status);
        return ret;
}

#ifdef POSTK_DEBUG_ARCH_DEP_87 /* move X86_IA32_xxx architecture-dependent */
int ihk_mc_counter_mask_check(unsigned long counter_mask)
{
	return 1;
}
#endif /* POSTK_DEBUG_ARCH_DEP_87 */
