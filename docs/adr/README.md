# Architecture Decision Records

ADRs document significant technical decisions. They are immutable —
supersede with a new ADR rather than editing an old one.

The first two ADRs live in `doc/ChainAPI - PRD.md` Appendix C:

- **ADR-001** — Qt 6 + C++ for the desktop app
- **ADR-002** — Two-phase architecture (in-process engine now,
  extractable later)

ADRs to be written (per PRD Appendix C):

- **ADR-003** — Why YAML schema over JSON / TOML / custom DSL
- **ADR-004** — Why local-first over cloud-first
- **ADR-005** — Why DAG-based execution over linear scripts
- **ADR-006** — Why open-core over pure-OSS or pure-SaaS

## Format

```markdown
# ADR-NNN: Title

## Status
Accepted | Proposed | Superseded by ADR-XXX
## Date
YYYY-MM-DD

## Context
What forces are at play? What problem are we solving?

## Decision
What we are doing.

## Consequences
- Positive
- Negative
- Risks
```
