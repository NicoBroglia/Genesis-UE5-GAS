# Genesis

**Online multiplayer souls-like built in Unreal Engine 5.6 with C++ and the Gameplay Ability System (GAS).**

> **Showcase repository.** This is the code-and-project version of Genesis. Third-party assets (Fab / Marketplace packs, Quixel Megascans and Epic template content) were removed from the whole history because their licenses don't allow public redistribution. The project opens in the editor, but meshes, animations and materials from those packs show up as missing references.

---

## Highlights

### Gameplay Ability System (multiplayer-ready)
- **Ability System Component on the PlayerState** for players, so abilities and attributes survive respawn and replicate correctly. Enemies own their ASC directly on the character.
- **Replicated attribute set** (`UGenesisAttributeSet`) with Health, MaxHealth, Mana and MaxMana, using `ReplicatedUsing` / `OnRep_` notifies and `GAMEPLAYATTRIBUTE_*` accessor macros.
- **Gameplay Effects:** `GE_InitializeStats`, `GE_Damage`, `GE_FullHeal`, `GE_StaminaRegen` and `GE_SprintDrain`.
- **Gameplay Abilities:** `GA_Sprint` with a stamina cost and regeneration.
- `IAbilitySystemInterface` is implemented on both the character base and the PlayerState.

### Characters and combat
- `AGenesisCharacterBase` is an abstract base class, with `AGenesisMainCharacter` and `AGenesisEnemyCharacter` built on it.
- Melee and ranged minion enemies, each with its own Animation Blueprint and Blend Space (idle/walk/run).
- `IEnemyInterface` handles enemy interaction.
- `AGenesisEffectActor` applies effects on overlap (pickups such as `BP_HealthPotion`), plus a damageable `BP_FireTrap` that works in multiplayer.
- Death and respawn flow, plus a HUD health bar (`WBP_HealthBar`).

### Souls-like feel
- Tuned `CharacterMovementComponent` settings: heavier gravity, separate braking friction, and turn rate that orients the character to movement.
- Tight over-the-shoulder spring-arm camera with lag and collision probing, built for narrow corridors.
- Enhanced Input player controller with configurable sensitivity, invert yaw/pitch and pitch clamping.

### Project organization
- Folder structure and naming conventions are documented below and followed throughout the project.
- Git workflow with feature and art branches merged through pull requests.

---

## Code map

```
Source/Genesis/
├── Public|Private/AbilitySystem/   GenesisAbilitySystemComponent, GenesisAttributeSet
├── Public|Private/Character/       GenesisCharacterBase, GenesisMainCharacter, GenesisEnemyCharacter
├── Public|Private/Player/          GenesisMainCharacterController, GenesisPlayerState
├── Public|Private/Actor/           GenesisEffectActor
├── Public|Private/Interaction/     EnemyInterface
└── Public|Private/Game/            GenesisGameModeBase
Content/Genesis/
├── GAS/                            Gameplay Effects and Abilities
├── Characters/                     Hero and enemy Blueprints, AnimBPs, Blend Spaces, input contexts
├── Blueprints/                     GameMode, PlayerState, pickups
├── UI/                             HUD widgets
└── Art/Environment/                Levels (LV1, LV1BIS) and level geometry
```

## Naming conventions

| Asset Type         | Prefix | Example          |
|--------------------|--------|------------------|
| Blueprint          | BP_    | BP_FireTrap      |
| Widget (UI)        | WBP_   | WBP_HealthBar    |
| Level (Map)        | L_     | L_TestGym        |
| Material           | M_     | M_StoneWall      |
| Material Instance  | MI_    | MI_StoneWall_Red |
| Gameplay Ability   | GA_    | GA_Dodge         |
| Gameplay Effect    | GE_    | GE_Damage        |
| Input Action       | IA_    | IA_Attack        |

## Opening the project
1. Install **Unreal Engine 5.6** and Visual Studio 2022 (with the "Game development with C++" workload) or JetBrains Rider.
2. Right-click `Genesis.uproject` → *Generate Visual Studio project files*, then build the `GenesisEditor` target.
3. Expect missing-asset warnings for the removed third-party content.

## Credits
- **Nicolás Broglia** ([@NicolasBroglia](https://github.com/NicolasBroglia)): gameplay programming, GAS architecture, C++ systems, project structure and documentation.
- **[@DLain47](https://github.com/DLain47)**: level art and level design (LV1, LV1BIS, fog walls, pillars).
- Part of the GAS foundation was built while following an Unreal Engine GAS course, then extended for this project.
