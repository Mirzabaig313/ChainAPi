# ChainAPI Desktop: Design System

The visual and interaction spec for the ChainAPI Qt 6.8 Widgets desktop app. This is the **Qt** design language: QSS stylesheets, `QPalette`, and `Theme` tokens, not web CSS. Source of truth for `desktop/src/theming/`, `desktop/src/views/`, and `desktop/src/widgets/`.

Register: **product** (a developer tool; design serves the task, not the other way around). Grounded in PRD §6.5 to §6.14 (UI requirements) and §7.5 (accessibility). Every token maps to a `Theme` field; every component maps to a class under `views/` or `widgets/`.

---

## 1. Design Principles

ChainAPI is a workbench for backend engineers and QAs, not a consumer app. The design follows from that:

1. **Information density over whitespace.** Engineers want many operations, headers, and response fields visible at once. Spacing is tight and deliberate.
2. **The chain is the hero.** The dependency chain and run timeline are the product's reason to exist. They get the strongest visual treatment; everything else recedes.
3. **State is legible at a glance.** Session validity, cache state, run status, and null extractions are all encoded in a consistent color plus glyph vocabulary, so a user scanning the panel knows the system state without reading text.
4. **Keyboard first.** Per NFR-5.1, no mouse-only flows. Every interactive element has a focus state and a shortcut. The command palette (Cmd+P) is the primary navigation.
5. **Native, not branded.** Respect each OS (macOS menu bar, Windows 11 Fluent accents, Linux GTK hints). ChainAPI does not impose custom chrome that fights the platform.
6. **Calm under load.** A running chain has a lot of motion potential. Restrain it: subtle progress, no scattered spinners. Motion communicates state change, nothing decorative.

### The product bar

The test is not "would someone say AI made this." Familiarity is a feature here. The test: would an engineer fluent in Linear, Raycast, or the JetBrains IDEs sit down and trust this, or pause at every subtly-off control? The tool should disappear into the task.

---

## 2. Color

Defined in `Theme.{h,cpp}`, applied via `light.qss` / `dark.qss`. All values are **OKLCH**. Chroma drops as lightness approaches 0 or 100, so highlights and shadows never look garish.

### 2.1 Color strategy

**Restrained** (the product floor): tinted neutrals carry the surface; a single committed accent appears only on primary actions, current selection, focus, and state indicators. The accent never decorates. One surface may earn a heavier accent moment (the live run timeline), but the resting app is quiet.

This is the 60-30-10 rule read as visual weight, not pixel count: ~60% tinted-neutral surfaces, ~30% secondary text and borders, ~10% accent. The accent works because it is rare; spending it on decoration kills its signal.

### 2.2 Token layers

Two layers, so dark mode only redefines the second:

- **Primitive tokens**: raw OKLCH ramps (`indigo.500`, `neutral.200`, `green.600`). Theme-independent.
- **Semantic tokens**: the names below (`accent.base`, `surface.raised`). Each maps to a primitive, and the mapping is what light/dark swaps. Widgets and QSS reference semantic names only, never primitives.

### 2.3 The accent, and why it is not blue

The first-order category reflex is "developer API tool, therefore dark blue." The second-order trap is "API tool that avoids blue, therefore terminal green on black." Both are training-data defaults; both are rejected here.

The accent is a committed **indigo-violet** (`oklch hue 285`). It reads as considered rather than defaulted, it does not collide with any status hue, and it carries the product's "chain of linked steps" identity without leaning on the blue or terminal-green clichés. Every neutral is tinted toward this same hue at very low chroma (0.004 to 0.010), so the whole surface feels intentional rather than gray.

Never use pure `#000` or `#fff`. The darkest text and lightest surface are both tinted toward hue 285.

### 2.4 Semantic tokens

Reference these names in QSS, never raw values. `Theme` resolves them per active mode.

| Token | Role |
|---|---|
| `surface.base` | Window background |
| `surface.raised` | Panels, content surfaces (one level up from base) |
| `surface.sunken` | Input fields, code editor gutter |
| `surface.overlay` | Popovers, command palette, tooltips |
| `border.subtle` | Panel dividers, default control borders |
| `border.strong` | Focused control border, active selection |
| `text.primary` | Body text, values |
| `text.secondary` | Labels, captions, metadata |
| `text.disabled` | Inactive controls |
| `text.inverse` | Text on accent fills |
| `accent.base` | Primary action, focus ring, current selection |
| `accent.hover` | Accent hover state |
| `accent.muted` | Accent backgrounds (selected row tint) |

### 2.5 Status palette

These map directly to engine state enums (`StepResult::Status`, `ActorSession::State`, `ErrorClass`). The same hue means the same thing everywhere: explorer badges, timeline nodes, graph highlights. Each hue is distinct from the others and from the accent.

| Token | Hue | Meaning | Maps to |
|---|---|---|---|
| `status.idle` | neutral | Pending, not run | `StepResult::Status::Pending` |
| `status.running` | 230 (cyan) | In flight | `Running`, `Authenticating`, `Refreshing` |
| `status.success` | 150 (green) | 2xx, extraction resolved | `Succeeded`, session `Live` |
| `status.warning` | 75 (amber) | Retry, slow, null extraction | retry attempts, `null` extract result |
| `status.error` | 27 (red) | Failure | `Failed`, `ErrorClass` non-`Run` |
| `status.cancelled` | neutral | User cancelled | `Cancelled` |
| `status.blocked` | 320 (magenta) | Upstream failed, never ran | `Blocked` |
| `status.skipped` | faint neutral | Dry run, conditionally skipped | `Skipped` |

`status.warning` is reserved for the null-extraction highlight required by FR-9.11, a non-error condition that still demands attention. Do not reuse it for failures. `status.running` (cyan, hue 230) is deliberately separate from the accent (indigo-violet, hue 285) so "the app wants your attention" and "this step is executing" never blur together.

### 2.6 Light theme values

```
surface.base     oklch(0.985 0.003 285)   surface.raised   oklch(0.995 0.002 285)
surface.sunken   oklch(0.960 0.004 285)   surface.overlay  oklch(0.995 0.002 285)
border.subtle    oklch(0.900 0.005 285)   border.strong    oklch(0.780 0.008 285)
text.primary     oklch(0.240 0.010 285)   text.secondary   oklch(0.470 0.009 285)
text.disabled    oklch(0.680 0.006 285)   text.inverse     oklch(0.985 0.002 285)
accent.base      oklch(0.550 0.190 285)   accent.hover     oklch(0.480 0.200 285)
accent.muted     oklch(0.950 0.030 285)
status.running   oklch(0.600 0.140 230)   status.success   oklch(0.560 0.150 150)
status.warning   oklch(0.700 0.150 75)    status.error     oklch(0.560 0.200 27)
status.cancelled oklch(0.550 0.010 285)   status.blocked   oklch(0.560 0.150 320)
```

### 2.7 Dark theme values

```
surface.base     oklch(0.180 0.006 285)   surface.raised   oklch(0.220 0.007 285)
surface.sunken   oklch(0.140 0.006 285)   surface.overlay  oklch(0.260 0.008 285)
border.subtle    oklch(0.300 0.008 285)   border.strong    oklch(0.420 0.010 285)
text.primary     oklch(0.940 0.004 285)   text.secondary   oklch(0.680 0.006 285)
text.disabled    oklch(0.480 0.006 285)   text.inverse     oklch(0.180 0.006 285)
accent.base      oklch(0.680 0.170 285)   accent.hover     oklch(0.750 0.160 285)
accent.muted     oklch(0.300 0.060 285)
status.running   oklch(0.750 0.130 230)   status.success   oklch(0.780 0.160 150)
status.warning   oklch(0.800 0.150 75)    status.error     oklch(0.700 0.180 27)
status.cancelled oklch(0.680 0.010 285)   status.blocked   oklch(0.720 0.160 320)
```

Verify every text and background pair with a contrast tool before committing a QSS change. Dark-mode `status.*` and `accent.*` values are intentionally brighter to clear 4.5:1 on `surface.raised`. Qt does not parse OKLCH directly; `Theme` converts these to sRGB hex at load time, so the source of truth stays perceptual while the QSS receives values Qt understands.

### 2.8 Dark mode is not inverted light mode

Dark mode is a separate set of decisions, not a color swap:

- **Depth comes from surface lightness, not shadow.** The three-step `surface.sunken < base < raised < overlay` ramp climbs in lightness; dark mode drops the drop-shadows light mode uses.
- **Body text weight drops to 350** in dark (from 400). Light text on dark reads heavier than dark on light, so the lighter weight keeps the visual color even.
- **Accents desaturate slightly** relative to a naive invert, already reflected in the §2.7 values.
- **Placeholder text still needs 4.5:1.** The washed-out gray placeholder is the most common contrast failure; `text.secondary`, not `text.disabled`, is the floor for placeholders.

### 2.9 Alpha is a design smell

Heavy transparency means an incomplete palette and produces unpredictable contrast. Define explicit tint tokens instead of `rgba` overlays:

| Token | Built from | Use |
|---|---|---|
| `tint.cache` | accent at the row-tint lightness | cached explorer rows (FR-5.4) |
| `tint.substituted` | accent at field-background lightness | resolved variable values (FR-6.2) |
| `tint.diffAdd` | success at low-saturation surface lightness | diff additions (FR-7.5) |
| `tint.diffRemove` | error at low-saturation surface lightness | diff removals (FR-7.5) |
| `tint.currentLine` | accent at gutter lightness | code editor current line |

Each is a precomputed opaque OKLCH value in `Theme`, not an alpha composite. The only sanctioned alpha is the focus ring and the `status.running` pulse, where see-through is the point.

---

## 3. Theme: light, dark, and the default

Dark versus light is never a reflex. The scene that forces the answer:

> A backend engineer debugging a multi-role auth chain at their desk in mid-afternoon office light, alt-tabbing between this and their IDE in focused 45-minute sessions.

That scene does not force dark or light on its own. What it forces is this: the tool sits beside an IDE the developer already themed. The honest default is therefore **System**, following the OS appearance the developer already chose. Both themes are first-class peers, not one plus an afterthought, which also satisfies the dual-theme contrast requirement (NFR-5.3).

Primary craft target is dark, since the plurality of backend developers run dark IDEs, but every screen is designed and contrast-checked in both. The toggle offers Light, Dark, System, stored in `QSettings`, applied live without restart.

---

## 4. Typography

Per NFR-5.4, all sizes respect OS font scaling. Never hardcode pixel sizes that ignore `QApplication::font()`. Use `Theme` text styles, derived from the system base size.

### 4.1 Families

System fonts are correct for a product UI. They give native feel on every platform and they are what the user's eye already expects.

| Use | Font | Fallback chain |
|---|---|---|
| UI (labels, menus, body) | System default | `.AppleSystemUIFont` (macOS), `Segoe UI` (Win), system sans (Linux) |
| Code, JSON, headers, URLs | Monospace | `SF Mono` (macOS), `Cascadia Code` (Win), `JetBrains Mono` bundled (Linux) |

Never ship a bundled UI font; it fights the native look. Monospace may be bundled (JetBrains Mono) because system monospace varies too much for the code editor and response viewer to stay consistent.

### 4.2 Type scale

Product UIs hold many type elements, so the scale ratio is tight (roughly 1.15 between steps). Exaggerated contrast would read as noise, not hierarchy. Hierarchy comes from weight as much as size. Sizes are relative to the system base, scaled by ratio rather than fixed pixels.

| Style | Ratio | Weight | Use |
|---|---|---|---|
| `title` | 1.30 | 600 | Panel headers, dialog titles |
| `subtitle` | 1.15 | 600 | Section headers within a panel |
| `body` | 1.00 | 400 | Default text, values |
| `label` | 0.92 | 500 | Field labels, tab text |
| `caption` | 0.85 | 400 | Metadata, timestamps, evidence strings |
| `mono` | 0.92 | 400 | Code, JSON, headers, JSONPath |

Line height is 1.4 for body and mono, tighter (1.15 to 1.2) for `title` and `subtitle`. Prose blocks (help text, evidence strings) cap at 65 to 75ch. Data surfaces (response tree, headers table) run denser; a 120ch table is fine.

### 4.3 Type details

- **`tabular-nums` for all aligned numbers:** status codes, durations, byte counts, extraction values in the timeline. Without it, columns of numbers jitter as digit widths vary. Set via the font's OpenType features on the `mono` and data styles.
- **Letter-spacing only where it earns it:** method chips and any uppercase label get slightly open tracking (roughly +0.02em); everything else stays at the font default. Never track lowercase body text.
- **Weights are role-locked.** Four weights total: 400 body (350 in dark, see §2.8), 500 labels, 600 headings, and the bundled mono at 400. The same role uses the same weight everywhere; a 600 heading in one panel and a 500 heading in another is a bug.
- **Semantic style names, not sizes.** Widgets reference `Theme` text styles (`title`, `body`, `caption`), never a raw point size.

---

## 5. Spacing & Layout

### 5.1 Spacing scale (px, pre-scaling)

Use the `Theme::space(n)` helper; never hardcode margins. As a workbench, default to the tighter end for intra-panel spacing and reserve the larger steps for section breaks. Vary spacing for rhythm; identical padding on every element reads as monotony.

| Token | px | Use |
|---|---|---|
| `space.xs` | 4 | Icon to text, badge padding |
| `space.sm` | 8 | Control padding, list row vertical |
| `space.md` | 12 | Field gaps, panel inner margin |
| `space.lg` | 16 | Section gaps |
| `space.xl` | 24 | Panel to panel (rare; panels usually share a splitter) |
| `space.xxl` | 32 | Empty-state centering only |

### 5.2 The three-pane workbench

```
+--------------+--------------------------+------------------+
| Project      |  Request Editor          |  Response Viewer |
| Explorer     |  (resolved request +     |  (JSON tree /    |
| (actors +    |   chain preview)         |   raw / headers  |
|  resources   |                          |   / diff)        |
|  tree)       +--------------------------+------------------+
|              |  Timeline (executed chain, per-step r/r)    |
+--------------+---------------------------------------------+
```

- Left, center, and right via a horizontal `QSplitter`. The timeline sits in a nested vertical `QSplitter` under center and right.
- Splitter handles use `border.subtle`, 1px, with a 4px hit target.
- Default ratio 22 / 44 / 34. Persist the user's splitter sizes to `QSettings`.
- Minimum panel widths prevent collapse to unusable. A panel hides entirely via the View menu, not by dragging to zero.

Familiar structure is an affordance here. Top-bar plus side-nav plus tabs are expected patterns; do not reinvent them for flavor.

### 5.3 Density modes

Two modes via the View menu, stored in `QSettings`:

- **Comfortable** (default): list rows at `space.sm` vertical padding.
- **Compact**: list rows at `space.xs`, for users with 500+ operations who want maximum visible.

---

## 6. Components

Each maps to a class under `views/` (composite panels) or `widgets/` (reusable atoms). Every interactive component ships with all of: default, hover, focus, active, disabled, loading, error. Shipping half of these is shipping a bug.

### 6.1 StatusBadge (`widgets/StatusBadge`)

The most-used atom. A small pill or dot encoding a `status.*` value.

- **Never color alone.** Pair each status with a glyph: check for success, filled dot for running, cross for error, slashed circle for cancelled, pause for blocked, triangle for warning or null. Color-blind users must distinguish states (the strongest accessibility rule in this app).
- Two variants: `Dot` (8px, for tree rows) and `Pill` (text plus glyph, for timeline and headers).
- `status.running` carries a subtle 1.2s opacity pulse, the one ambient animation in the app. It stops the instant state changes.

### 6.2 Project Explorer (`views/ProjectExplorerWidget`), FR-5

- `QTreeView` with a custom delegate. Two top-level groups: Actors (each with a session `StatusBadge`) and Resources (each expands to operations with method chips).
- Method chips: colored mono text on `surface.sunken`, uppercase. `GET` accent, `POST` success hue, `DELETE` error hue, `PUT` and `PATCH` warning hue. Muted, not loud.
- **Cache state (FR-5.4):** a cached row carries a faint full-width `tint.cache` row fill plus a small cache glyph in the trailing column. (No side accent border; a colored left stripe is a banned pattern and reads as decoration rather than meaning.)
- Search field (FR-5.2) pinned to the top, filters as you type. Cmd+F focuses it.
- Right-click menu (FR-5.3): Run, Run with Override, Edit Schema, View Dependencies.

### 6.3 Request Editor (`views/RequestEditorPanel`), FR-6

- **Read-only by default** (FR-6.1). The panel reads as a preview, not a form: `surface.sunken` fields, no edit affordance until Override Mode.
- Override Mode toggle in the panel header. When on, fields become editable with a `border.strong` outline and a persistent "Override active, one-shot" banner tinted `status.warning`.
- Resolved-request view (FR-6.2): shows the request after variable substitution. Substituted values get a subtle `tint.substituted` background so the user sees what came from where.
- Chain preview (FR-6.3): a collapsed-by-default list of the steps that will execute, each with a `status.idle` badge.
- Two buttons: **Send** (`accent.base`, primary) and **Send Cleanly** (secondary, `border.strong` outline). Cmd+Enter sends; Cmd+Shift+Enter sends cleanly.

### 6.4 Response Viewer (`views/ResponseViewerPanel`), FR-7

- Tabbed: Tree, Raw, Headers, Diff.
- Tree (FR-7.1): a collapsible `JsonTree` widget, monospace. Click a value to copy it as JSONPath (FR-7.4), with a transient toast confirming the copied path.
- Raw (FR-7.2): `CodeEditor` (QScintilla) in read-only mode with JSON highlighting from the theme palette.
- Diff (FR-7.5): two-up or inline, additions filled `tint.diffAdd`, removals filled `tint.diffRemove`, both light enough that text stays readable.
- Status line at top: method chip, status code in the matching `status.*`, duration in `caption`.

### 6.5 Timeline (`views/TimelinePanel`), FR-7.6

- A horizontal sequence of step nodes, each a `StatusBadge.Pill` plus operation name: the executed chain, left to right.
- Click a node to load that step's request and response in the viewer above.
- **Extraction values (FR-9.11):** below each node, show extracted values. `null` results highlight in `status.warning` with a connector drawn to the downstream consumers that depended on them. This is the timeline's most important job; make it prominent.
- During a run, the active node pulses `status.running`; completed nodes settle to their terminal status. This is the one surface allowed a heavier accent moment.

### 6.6 Dependency Graph (`views/DependencyGraphView`, Phase 2), FR-8

- A `QQuickWidget` hosting a QML graph (the one place QML appears; see `qml/DependencyGraph.qml`).
- Nodes use the same `status.*` palette. The execution path highlights from `status.running` to the terminal color (FR-8.3).
- Circular dependencies (FR-8.4) draw with `status.error` edges and a warning banner.

### 6.7 Command Palette (`widgets/CommandPalette`), FR-14

- Cmd/Ctrl+P. `surface.overlay`, centered, 600px max width, soft shadow.
- Fuzzy find over operations; recent at the top (FR-14.3). A `>` prefix switches to global commands (FR-14.4).
- Keyboard only: arrows navigate, Enter runs, Esc closes. The selected row uses `accent.muted`.

### 6.8 CodeEditor (`views/CodeEditor`)

- A QScintilla wrapper for YAML, JSON, and JS hook editing. The syntax theme derives from `Theme`; no QScintilla default colors.
- Gutter on `surface.sunken`. Current line filled `tint.currentLine`. Matches the `mono` type style.

---

## 7. Cognitive Load

A workbench shows a lot at once. The risk is not blankness, it is overload. The job is to spend the user's working memory only on the task (the chain), never on decoding the interface. Working memory holds about four items at once (Miller, revised by Cowan); design every decision point to stay under that.

### 7.1 The three loads

- **Intrinsic** (the API testing task itself): cannot be removed, only structured. Group an operation with its actor, its dependencies, and its extractions so the user reasons about one chain at a time.
- **Extraneous** (bad design): eliminate ruthlessly. Inconsistent badges, mystery icons, a Send button that looks different in two panels. Pure waste.
- **Germane** (learning the tool): support it. Consistent patterns reward the user for learning once, then applying everywhere.

### 7.2 Working-memory budget per surface

| Surface | Keep under |
|---|---|
| Explorer top-level groups | 2 (Actors, Resources). Everything else nests. |
| Primary actions per panel | 1 primary (`accent.base`) + at most 2 secondary; the rest go in the context menu or palette |
| Request Editor sections visible at once | 4 before a visual break (method+URL, headers, body, chain preview) |
| Chain-preview steps shown expanded | collapse by default; expand on demand |
| Tabs in the Response Viewer | 4 (Tree, Raw, Headers, Diff) |

When a surface needs more, **group or progressively disclose**. Never show a wall of options.

### 7.3 Progressive disclosure

- Request Editor is read-only until Override Mode is requested. The edit affordances do not exist until needed.
- Chain preview is collapsed; the user expands it when they want to see the steps.
- Per-field AI evidence strings (FR-9.9) are hidden behind a disclosure on each imported field, not shown inline by default.
- Advanced run options (force re-run scope, retry override) live in a context menu, not the main panel.

### 7.4 Co-locate, never make the user remember

The Memory Bridge is the worst load for this tool: forcing the user to remember a value from one panel to act in another. Counter it:

- The resolved request (FR-6.2) shows substituted values in place, so the user never holds "what did `{{auth.token}}` resolve to" in their head.
- The timeline shows each step's extracted values with connectors to downstream consumers (FR-9.11), so the data flow is on screen, not in memory.
- Clicking a timeline node loads that step's full request and response in the viewer above, co-locating everything needed to judge one step.

### 7.5 The squint test

Blur the screen (or screenshot and blur). The running step, the failed step, and the primary action must still be identifiable. If every panel reads at the same visual weight when blurred, the hierarchy has failed. Fix with weight, color, and space together, never size alone.

---

## 8. Iconography

- One consistent icon set (SF Symbols on macOS where available; a bundled set such as Lucide for cross-platform consistency elsewhere).
- 16px default, 20px for the toolbar, scaled with OS font scaling.
- Icons are `text.secondary` by default, `text.primary` on hover or active, `accent.base` when representing the active primary action.
- Status glyphs (§6.1) are the exception; they carry their `status.*` color and never appear without it.
- Geometrically centered glyphs often look off-center; nudge play and arrow glyphs toward their direction so they read as centered (optical, not geometric, alignment).

---

## 9. Motion

Minimal and functional, in keeping with principle 6. Product motion conveys state, never decoration, and stays in the 120 to 250ms range so users in flow never wait on choreography.

| Element | Motion | Duration |
|---|---|---|
| `status.running` badge | opacity pulse 0.6 to 1.0 | 1.2s loop |
| Panel show or hide | width and opacity ease | 150ms |
| Command palette | fade plus 4px rise | 120ms |
| Toast (copied JSONPath) | fade in and out | 100ms in, 1.5s hold, 200ms out |
| Tab switch | none (instant) | n/a |
| Tree expand or collapse | native Qt default | n/a |

Ease out with an exponential curve (ease-out-quart or quint). No bounce, no elastic, no spring. No spinners on individual rows; a running chain shows progress through the timeline, not scattered loaders. No orchestrated load sequence; the app loads into a task. If the OS requests reduced motion, disable the pulse and all transitions.

---

## 10. States Every Component Must Handle

Define all of these for each interactive widget:

- **Default:** resting.
- **Hover:** `text.primary` or `accent.hover`, pointer cursor.
- **Focus:** `border.strong` ring, keyboard reachable. Never remove focus outlines (NFR-5.1).
- **Active or pressed:** momentary darken.
- **Selected:** `accent.muted` background.
- **Disabled:** `text.disabled`, no pointer events, a `caption`-level explanation when non-obvious. No heavy or full-saturation color on inactive states.
- **Loading:** for async-bound widgets, a skeleton state, not a spinner in the middle of content, and never a blocking modal spinner.
- **Empty:** first-run and no-data states get a centered message plus a primary action ("Import an API", "Open a project"), and they teach the interface rather than saying "nothing here." See PRD §12.
- **Error:** inline, `status.error`, with the `ErrorCode` string (`toCodeString`) and a human message. Never a raw stack trace.

Hover and focus are different states for different users. Keyboard users never see hover. Design both; never ship one as a stand-in for the other.

---

## 11. Interaction & Keyboard

Per NFR-5.1 the app is fully keyboard-operable. These patterns translate the web focus and overlay rules into Qt terms.

### 11.1 Focus, not hover, is the contract

- Every interactive widget has a visible focus ring: `border.strong`, 2px, drawn outside the control, 3:1 against its surroundings. Qt's `:focus` pseudo-state in QSS, never suppressed.
- Mouse hover may add a subtle shift, but focus is the state that must always be visible, because keyboard and screen-reader users live in it.
- Tab order follows reading order. Set it explicitly with `setTabOrder` where the widget tree does not already match.

### 11.2 Roving focus inside groups

For the explorer tree, the timeline node strip, and the response tabs, use Qt's native group navigation: arrow keys move within the group, Tab leaves it. Do not make every node a separate tab stop, that turns a 50-step chain into 50 tabs.

### 11.3 Overlays and stacking

Command palette, context menus, and tooltips are top-level popups (`Qt::Popup` / `QMenu` / `QToolTip`), so they escape panel clipping and stack correctly without manual z-fighting. Define a semantic stacking order and keep to it:

```
panel  <  splitter handle  <  dropdown/menu  <  command palette  <  toast  <  tooltip
```

Light-dismiss (click outside closes, Esc closes) is mandatory for the palette and menus. Esc also cancels a running chain per PRD §9.3, so palette-open Esc closes the palette first, then a second Esc reaches the run.

### 11.4 Destructive actions: undo over confirm

Prefer an undo toast to a confirmation dialog. "Reset Cache", "Delete Environment", and similar remove immediately and show a 5s undo toast; the real delete happens when the toast expires. Reserve a blocking confirm only for the truly irreversible (deleting a project file on disk). Modals are the last resort, not the first thought (per the shared bans).

### 11.5 Touch and hit targets

Even on desktop, interactive targets are at least 28px tall (Qt desktop convention; the 44px web touch minimum relaxes for mouse-driven density, but icon-only buttons get padding to a comfortable click target). Visual size and hit target are separate; a 16px icon button still claims a larger clickable area.

---

## 12. Accessibility Checklist (NFR-5)

Every PR touching `desktop/` verifies:

- [ ] All controls reachable and operable by keyboard (NFR-5.1). Tab order is logical.
- [ ] Every control has an accessible name via `setAccessibleName` and `setAccessibleDescription` (NFR-5.2).
- [ ] Text and background pairs meet WCAG 2.1 AA in both themes (NFR-5.3), verified with a contrast tool, not by eye.
- [ ] No information conveyed by color alone; status always pairs color with a glyph (§6.1).
- [ ] Placeholder text meets 4.5:1 (use `text.secondary`, never `text.disabled`).
- [ ] Layout survives OS font scaling to 200% without clipping (NFR-5.4).
- [ ] Focus indicators visible and never suppressed.
- [ ] Hover and focus designed separately; keyboard users get a focus state on every control.
- [ ] Screen reader announces run start and finish via accessible state updates.

---

## 13. Implementation Notes

- **Tokens, not raw values.** All color, spacing, and type live in `Theme`. QSS references token-derived values via `Theme`-generated stylesheet strings or dynamic properties. A raw hex or OKLCH literal in a `.qss` or `.cpp` is a review failure.
- **Two token layers.** Primitive OKLCH ramps, then semantic names that map to them (§2.2). Dark mode redefines only the semantic mapping. Widgets touch semantic names exclusively.
- **Tint tokens, not alpha.** The `tint.*` tokens (§2.9) are precomputed opaque values. Do not reintroduce `rgba`/alpha composites for row highlights, diffs, or substituted values.
- **OKLCH at the source, sRGB at the edge.** `Theme` stores perceptual values and converts to sRGB hex once at load, since Qt's QSS parser does not read OKLCH.
- **`ThemeExtension` pattern.** Custom tokens not covered by `QPalette` (status colors, spacing scale) live on the `Theme` object, injected at app start in `Bootstrapper`.
- **Theme toggle.** Light, Dark, System, stored in `QSettings`, applied without restart by reloading the active `.qss` and repolishing widgets (`style()->unpolish` then `polish`).
- **No inline styles.** Per-widget `setStyleSheet` is banned except for genuinely one-off cases; prefer object-name selectors in the central QSS.
- **Theme and density are runtime, not compile-time.** Both switch live.
- **Semantic stacking order**, not arbitrary `raise()` calls: `panel < splitter handle < dropdown/menu < command palette < toast < tooltip` (per §11.3). Top-level popups handle this naturally; do not hand-tune stacking with magic values.
- **Elevation is subtle.** In light mode, a shadow you can clearly see is too strong; use the smallest elevation that separates the surface. In dark mode, separate by surface lightness (§2.8), not shadow.
- **`gap`/layout spacing over per-widget margins** where Qt layouts allow it, so spacing stays consistent and centralized.

---

## 14. Anti-Reflex Notes (Keep This Tool From Looking Defaulted)

A short list so future contributors do not drift back to category defaults:

- The accent stays indigo-violet (hue 285). Do not "simplify" it back to SaaS blue.
- `status.running` stays cyan (hue 230), distinct from the accent. Do not merge them.
- Neutrals stay tinted toward 285. Do not flatten to pure gray, and never use `#000` or `#fff`.
- No colored side stripes on rows, cards, or alerts. Use full borders, background tints, leading glyphs, or nothing.
- No gradient text, no decorative glassmorphism, no hero-metric template.
- Cards are the lazy answer; this app is mostly panels and trees, so reach for a card only when it is genuinely the right affordance, and never nest them.

---

## 15. Reference

- PRD §6.5 to §6.14 (UI requirements), §7.5 (accessibility), §12 (first-run).
- Project Layout §1 (`desktop/` tree), §2.5 (desktop CMake target).
- `AGENTS.md`: use `/qt-reviewer` for any change here; `/qt-patterns` skill for Qt 6.8 idioms.
- Engine state enums this design maps to: `engine/include/chainapi/engine/RunContext.h` (`StepResult::Status`, `ActorSession::State`), `ErrorCodes.h` (`ErrorClass`).
