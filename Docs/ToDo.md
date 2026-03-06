# DCP06 – Next Items / ToDo

Items to address in upcoming work.

---

## Database migration: Replace ADF/file checks with database handling

**Context:** There are many places in the code that check if an ADF file is open (and possibly other DCP05-type files). With the database and `*.job` files in the database, we no longer need to check if ADF or other DCP05-type files are open.

**Current state:**
- Code checks for ADF file open/closed in many locations
- May also check other DCP05-type files
- This is legacy DCP05 file-based approach

**Target state:**
- Replace file-open checks with database checks, writes, and reads
- Use DCP9 as reference for how database and job handling are done
- Align with DCP9’s database access patterns

**Reference:** DCP9 codebase at `C:\Users\dell\Desktop\AMS\Development\DCP9`

**Action:** Audit codebase for ADF/file-open checks. Replace with database-centric logic (job open, active job, read/write via DB). Use DCP9 for patterns and conventions.

---

## Logging / tracing for simulator and debugging

**Context:** DCP9 has a good logging mechanism even for release builds. DCP06 needs similar capability to trace what is happening during development and testing.

**Priority:** High – needed for Leica Captivate simulator testing (starting soon). Need detailed visibility into execution flow, state changes, and errors.

**Scope (phased):**
1. **Phase 1:** Implement logging for Leica Captivate simulator usage. Enable detailed tracing to understand behavior during simulator testing.
2. **Phase 2 (later):** Decide whether logging is needed for target total station (instrument) builds. May be disabled or reduced for production if not required.

**Reference:** DCP9 codebase at `C:\Users\dell\Desktop\AMS\Development\DCP9` – review DCP9’s logging mechanism and adapt for DCP06/Captivate.

**Action:** Add logging/tracing infrastructure. Use DCP9 as reference. Ensure it works in release builds for simulator. Support configurable verbosity (e.g. debug, info, warning, error).

---

*Last updated: March 2025*
