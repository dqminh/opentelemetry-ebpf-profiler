// Provides helpers for collecting Last Branch Record (LBR) data from the CPU's
// branch sampling hardware via bpf_read_branch_records().
//
// Supported hardware paths:
//   - LBR for Intel and AMD LBRv2 for Zen4+
//   - AMD BRS

#ifndef OPTI_LBR_H
#define OPTI_LBR_H

#include "bpfdefs.h"
#include "kernel.h"

// collect_lbr_stack fills `out` with the CPU's last branch records and
// returns the number of entries written. Returns 0 if the kernel helper
// is unsupported, if the perf event was not opened with
// PERF_SAMPLE_BRANCH_STACK, or if the hardware has no branches to report.
static EBPF_INLINE int collect_lbr_stack(void *ctx, struct perf_branch_entry *out)
{
  long written =
    bpf_read_branch_records(ctx, out, MAX_BRANCH_RECORDS * sizeof(struct perf_branch_entry), 0);
  if (written < 0) {
    return 0;
  }
  return (int)(written / sizeof(struct perf_branch_entry));
}

#endif // OPTI_LBR_H
