# Third-Party Code

Reserved for vendored dependencies. Currently empty.

All managed dependencies live in `vcpkg.json` at the repository root.

## Planned vendored deps

- **QuickJS** — sandboxed JS interpreter for pre/post hooks
  - Why vendor: small (~600 KB), single-file, slow-moving, avoids
    pinning issues across vcpkg/Conan.
  - Where: `third_party/quickjs/`
  - When: Phase 1, alongside hook-runner implementation.
