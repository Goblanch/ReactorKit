# ReactorKit

> UNDER DEVELOPMENT

**Trigger. Evaluate. Act. Build anything.**

ReactorKit is an open source **Event-Condition-Action (ECA)** system plugin for Unreal Engine 5.  
Design complex game logic using Triggers, Conditions, and Actions — fully extensible from both Blueprint and C++,  
with no genre-specific assets. Works for any type of game.

> Developed and maintained by [Mirador Works](https://github.com/Goblanch) · [FAB Marketplace](#) *(coming soon)*

---

## Features

- **Modular ECA architecture** — Triggers detect events, Conditions evaluate them, Actions respond
- **AND / OR / NOT condition trees** — combine conditions with full boolean logic
- **Chainable actions with delays** — build sequences of actions with configurable timing
- **Central event bus** — decoupled communication via `URKDispatcherSubsystem` (GameInstanceSubsystem)
- **ReactorKit Editor Tab** — visual overview and configuration of all Reactor Actors in the active level
- **Blueprint & C++ extensible** — every base class uses `BlueprintNativeEvent` for seamless extension
- **State persistence** — lightweight save/load via UE5 native SaveGame system
- **Zero genre lock-in** — no horror, RPG, or shooter assets included

---

## Requirements

| Requirement | Version |
|-------------|---------|
| Unreal Engine | 5.7 |
| Project type | C++ (required to compile from source) |
| Platform | Windows / Mac |

---

## Installation

### From source

1. Clone this repository:

```bash
git clone https://github.com/Goblanch/ReactorKit.git
```

2. Symlink or copy the folder into your UE5 project's `Plugins/` directory:

**Windows** *(run terminal as Administrator)*:
```bat
mklink /D "C:\Path\To\YourProject\Plugins\ReactorKit" "C:\Path\To\ReactorKit"
```

**Mac / Linux**:
```bash
ln -s "/path/to/ReactorKit" "/path/to/YourProject/Plugins/ReactorKit"
```

3. Right-click your `.uproject` file → **Generate Visual Studio project files**
4. Compile the project
5. Open the UE5 Editor → **Edit > Plugins** → search for `ReactorKit` → enable it

### From FAB Marketplace

*(Coming soon)*

---

## Architecture overview

```
┌─────────────────────────────────────────────────────┐
│                  ARKReactorActor                    │
│                                                     │
│  ┌──────────────────┐   ┌────────────────────────┐  │
│  │  Triggers[]      │   │  ConditionTree         │  │
│  │  Overlap         │   │  AND / OR / NOT        │  │
│  │  Input           │   │  + Conditions[]        │  │
│  │  Timer           │   └────────────────────────┘  │
│  │  Custom          │                               │
│  └──────────────────┘   ┌────────────────────────┐  │
│                         │  ECAChain              │  │
│                         │  Action → delay        │  │
│                         │  Action → delay        │  │
│                         │  Action                │  │
│                         └────────────────────────┘  │
└──────────────────────────────┬──────────────────────┘
                               │ DispatchEvent()
                               ▼
┌─────────────────────────────────────────────────────┐
│            URKDispatcherSubsystem                   │
│             (GameInstanceSubsystem)                 │
│  RegisterListener · UnregisterListener · Dispatch   │
└─────────────────────────────────────────────────────┘
```

The plugin is split into two modules:

- **`ReactorKitCore`** — runtime logic, base classes, subsystem, persistence. Included in all builds.
- **`ReactorKitEditor`** — editor tools, ReactorKit Tab, property customizations. Editor-only, excluded from shipping builds.

---

## Quick start

### 1. Place a Reactor Actor

Drag an `ARKReactorActor` into your level from the Place Actors panel.

### 2. Configure from the Details panel

No Blueprint required. Select the actor and configure:
- Add one or more **Triggers** (Overlap, Input, Timer, or Custom)
- Build a **Condition Tree** (optional)
- Add **Actions** to the ECA Chain

### 3. Open the ReactorKit Editor Tab

Go to **Tools > ReactorKit** to see a full overview of all Reactor Actors in the active level.  
Enable, disable, and inspect them without touching the viewport.

### 4. Extend from Blueprint or C++

Create a Blueprint child of any base class to add custom logic:

| Base class | Purpose |
|---|---|
| `URKTriggerComponent` | Custom trigger logic |
| `URKConditionBase` | Custom condition evaluation |
| `URKActionBase` | Custom action execution |

---

## Project structure

```
ReactorKit/
├── ReactorKit.uplugin
├── Source/
│   ├── ReactorKitCore/          # Runtime module
│   │   ├── Public/
│   │   │   ├── Triggers/
│   │   │   ├── Conditions/
│   │   │   ├── Actions/
│   │   │   ├── Chain/
│   │   │   ├── Subsystem/
│   │   │   ├── SaveGame/
│   │   │   └── Library/
│   │   └── Private/
│   └── ReactorKitEditor/        # Editor-only module
│       ├── Public/
│       │   ├── Tab/
│       │   └── Customizations/
│       └── Private/
└── Content/
    └── Demo/
        └── Maps/                # Demo level
```

---

## Roadmap

| Version | Planned feature |
|---------|----------------|
| **v1.0** | Core ECA system, Editor Tab, persistence, demo map |
| v1.1 | Visual node graph editor |
| v1.2 | Multiplayer replication support |
| v1.3 | UE 5.4 / 5.5 compatibility |
| v2.0 | AI / Behavior Tree integration |

---

## Contributing

Contributions are welcome!  
Please read [CONTRIBUTING.md](CONTRIBUTING.md) before opening a PR.

- Bug reports → [Issues](https://github.com/Goblanch/ReactorKit/issues/new/choose)
- Feature requests → [Issues](https://github.com/Goblanch/ReactorKit/issues/new/choose) or [Discussions](https://github.com/Goblanch/ReactorKit/discussions)
- Questions → [Q&A Discussions](https://github.com/Goblanch/ReactorKit/discussions/categories/q-a)

---

## License

MIT License — see [LICENSE](LICENSE) for full details.

---

<p align="center">
  Made with care by <a href="https://github.com/Goblanch">Mirador Works</a>
</p>