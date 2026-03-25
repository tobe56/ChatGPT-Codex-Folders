# RDR2 Epilogue RPG Overhaul World + UI Draft

## Direction Lock
This overhaul stays entirely inside the existing Red Dead Redemption 2 world.
No brand-new landmass is required.
All RPG content is layered onto the epilogue map using existing towns, ranches, caves, forts, rail lines, gang hideouts, forests, swamps, mines, churches, burned farms, and abandoned buildings.

That keeps the tone believable, cuts implementation risk, and lets the mod feel like a hidden second life for the base game instead of a detached expansion pack.

## Core World Structure

### Main Play Space
The mod uses:
- West Elizabeth
- New Austin
- New Hanover
- Lemoyne
- Ambarino

Guarma and hard chapter-locked story spaces are not required for the core loop.

### Content Philosophy
Every system should feel like it belongs to the frontier after the epilogue.

The map is reinterpreted as:
- hunting grounds
- relic routes
- outlaw territories
- elite target zones
- boss arenas
- material farming loops
- house and workshop progression spaces

### Existing Locations Reused As RPG Content
- Beecher's Hope: player hub, ledger hub, upgrade station, stash, display room
- Blackwater: contracts, trade, rare goods, faction pressure
- Armadillo / Tumbleweed: dangerous late-game frontier economy
- Fort Mercer / gang hideouts: elite clears, bosses, salvage
- mines / caves / ruined churches / burned towns: relic dungeons and rare farming points
- trapper / gunsmith / fence-adjacent locations: upgrade loops and material sinks

## Player Hub: Beecher's Hope

### Why Beecher's Hope
John already has a home in the epilogue.
That makes it the cleanest place to anchor RPG systems without breaking immersion.

### What Beecher's Hope Becomes
Beecher's Hope turns into the player's long-term progression hub.

It contains:
- Frontier Ledger table
- upgrade bench
- relic stash chest
- bounty and target wall
- accessory display shelf
- boss trophy board
- map intelligence board

### In-World Logic
The ledger is where John tracks the new frontier war.
The bench is where upgrades are installed.
The stash holds relics, boss drops, and rare materials.
The wall and board let the player see what regions are getting worse and which targets are active.

## UI Direction

## Primary Goal
The player must be able to check:
- active questlines
- repeatable content
- stats
- perks
- upgrades
- accessories
- known boss locations
- discovered crafting materials
- region danger
- progression objectives

without leaving immersion.

## UI Approach: Two-Layer System

### Layer 1: Frontier Ledger UI
This is the main RPG interface.
It is styled like a field ledger / satchel notebook rather than a modern floating menu.

It should feel like:
- paper pages
- region tabs
- pinned notes
- sketches
- target portraits
- hand-marked maps

### Layer 2: Beecher's Hope Hub Interaction
At Beecher's Hope, the player can walk to the Ledger table and open the full management version of the UI.

This version includes:
- full quest board
- homefront requests
- affinity stage quests
- all stats and perks
- full accessory management
- boss archive
- long-form region intel
- upgrade and crafting queue
- stash and trophy review

## Input / Access Method

### Preferred Method
Use a dedicated hotkey to open the Frontier Ledger anywhere in free roam.

Default concept:
- `F10` opens the compact Frontier Ledger

This is the most reliable implementation path.

### Immersive Method
At Beecher's Hope, interacting with the ledger table opens the full hub version.

### Satchel-Style Fallback
If a fully custom overlay proves awkward, use a satchel-like page flow:
- open the ledger with a hotkey or house interaction
- flip pages left and right like a book
- separate tabs for Quests / Map Intel / Homefront / Affinity / Stats / Perks / Crafting / Bosses / Collection

### Inventory-Key Style Option
If you want it to feel closer to the base game, the ledger can optionally be mapped to the normal inventory/satchel behavior:
- outside Beecher's Hope, a hotkey opens a lighter field version
- inside Beecher's Hope, pressing the same inventory-style key near the ledger desk opens the RPG ledger instead

That keeps the interaction feeling "native" without needing to replace the entire vanilla satchel everywhere.

## Recommended Final UI Choice
Use this exact structure:

1. Anywhere in the world:
`F10` opens Compact Frontier Ledger

2. At Beecher's Hope, near the ledger desk:
interaction prompt opens Full Frontier Ledger

3. Inside the ledger:
left/right page flip navigation, satchel-like presentation

This is the best mix of immersion, usability, and implementation safety.

## UI Tabs

### 1. Active Trails
This is the quest tab.

It shows:
- main storyline stage
- active side jobs
- elite hunts
- boss hunt chains
- region events
- repeatable content contracts

### 2. Frontier Map
This is the intelligence tab.

It shows:
- marked danger zones
- active boss territories
- elite spawn zones
- relic dig or search areas
- known faction patrol routes
- region control status

### 3. John
This is the stats tab.

It shows:
- Strength
- Grit
- Deadeye
- Survival
- Cunning
- total level
- current bonuses

### 4. Homefront
This is the ranch support tab.

It shows:
- Abigail / Jack / Uncle current roles
- active Beecher's Hope requests
- current support risk
- what the next return-home interaction should solve

### 5. Affinity
This is the relationship tab.

It shows:
- visible score and current stage
- current or next affinity quest
- support route and recommended interaction
- repeatable payout bonus state

### 6. Perks
This is the progression tab.

It shows:
- unlocked perks
- locked perks
- tier branches
- perk point costs
- affinity-linked perk unlocks

### 7. Gear
This is the equipment tab.

It shows:
- equipped accessories
- weapon modifiers
- outfit-linked bonuses
- charm and talisman slots

### 8. Workshop
This is the upgrade tab.

It shows:
- available upgrades
- required materials
- missing materials
- installed enhancements

### 9. Ledger of the Dead
This is the boss tab.

It shows:
- discovered bosses
- unknown silhouettes for undiscovered bosses
- last known territory
- rewards
- lore note

### 10. Collection
This is the long-term completion tab.

It shows:
- relic sets
- rare materials
- faction tokens
- legendary trophies
- display completion

## Beecher's Hope Interaction Rhythm
The desk interaction should feel like a return-home ritual instead of only a menu open.

Recommended order after a field run:
1. open the full ledger at the table
2. check homefront requests and affinity quest states
3. review perk and crafting unlocks opened by the last sweep
4. decide whether to push another route or stabilize the ranch first

## On-Screen Minimal HUD
The mod should not flood the screen.
The main world HUD additions should stay light:

- region threat meter
- small quest update toast
- boss warning banner
- material pickup notice
- elite enemy title card

Everything else stays inside the ledger.

## How Quests And Content Appear In The Existing World

### Main Storyline
The main RPG storyline unfolds through:
- courier notes
- bounty leads
- campfire rumors
- Blackwater contacts
- ledger updates

### Repeatable Content
Repeatable loops appear as:
- contract hunts
- relic recovery jobs
- escort and interception jobs
- outlaw clearouts
- elite tracker missions
- supply raids

### Boss Encounters
Bosses should not just be standing in the open.
They are unlocked through:
- rumors
- item clues
- faction escalation
- chained objectives
- night-only or weather-only triggers

## Why Stronger Enemies Work In The Existing World
We do not need fantasy armies from nowhere.

The stronger enemies are:
- frontier mercenaries
- black-market hunters
- cultists
- relic smugglers
- old gang survivors
- bounty veterans
- region warbands

Later enemies can lean into myth and fear, but they should still emerge from places the player already knows.

## The "Flying Monster" Problem
This should be introduced carefully.

### Recommended Tone
Do not make the first flying threat a literal cartoon dragon.

Instead:
- early rumors describe a sky serpent
- some sightings may be staged or misidentified
- late-game revelation can become more supernatural or monstrous

This preserves the Red Dead tone while still giving you the huge "what the hell is that" moment you want.

## First-Stage Implementation Recommendation
For the first playable version, build this order:

1. Beecher's Hope hub
2. Compact Frontier Ledger
3. Stats and perks
4. repeatable contracts
5. accessory and upgrade system
6. elite enemies
7. first named boss
8. late-game mythic boss chain

That order gives the mod identity fast without needing the wildest content first.

## Practical Recommendation
Use Beecher's Hope as the physical anchor.
Use a ledger UI as the main menu.
Use the existing world as the entire RPG map.
Use a hotkey plus an in-world desk interaction.

That is the most believable and most buildable version of this idea.
