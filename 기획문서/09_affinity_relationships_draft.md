# RDR2 Epilogue RPG Overhaul Affinity And Relationship Systems Draft

## Core Goal

Add a grounded relationship progression layer for major epilogue NPCs without turning the game into a dating sim or a pure visual-novel system.

The relationship system should:

- reinforce Beecher's Hope as a living hub
- reward roleplay and long-term investment
- connect dialogue, gifts, quests, and growth systems
- give repeatable NPCs a stronger economic identity

## Initial Priority NPCs

The first pass should focus on:

- Abigail
- Jack
- Uncle

These are the highest-value targets because they already matter to the family, homefront, and ending pillars.

## Affinity Structure

Every affinity track has:

- a relationship score
- a visible progress bar
- a current stage
- stage rewards
- dedicated quest gates for stage rewards

## Stage Count

### Base Stages

Normal progression starts at:

- Stage 1
- Stage 2
- Stage 3

These stages should be reachable in the early and mid game.

### Late Stages

Additional late stages exist:

- Stage 4
- Stage 5

These stages are not normal early progression rewards.
They are gated by world tier growth and should feel like late-campaign loyalty payoffs.

Suggested gate:

- Stage 4 requires at least World Tier 2
- Stage 5 requires at least World Tier 4

## How Affinity Changes

Affinity should not rise from one source only.
It should come from multiple grounded inputs.

### Positive Sources

- giving affinity items
- choosing good dialogue options
- completing dedicated affinity quests
- meeting preferred stat or skill thresholds
- returning with relevant trophies, clues, or quest items

### Negative Sources

- choosing hostile, careless, or selfish dialogue options
- failing dedicated affinity quests
- ignoring relationship-critical requests for too long
- bringing home proof that John escalated danger recklessly

### Important Rule

Affinity can rise or fall.
The system should support stage loss if the score drops far enough.

## Affinity Items

Affinity items are not generic gifts only.
They should match the NPC.

Examples:

- Abigail: household supplies, medicine stock, practical keepsakes, ranch security upgrades
- Jack: sketches, books, relic notes, strange clues, archive materials
- Uncle: liquor, rumor payments, black-market tokens, easy-score salvage bundles

## Dialogue Role

Dialogue choices should matter mechanically.

They should:

- add affinity
- remove affinity
- unlock or block quests
- reinforce character tone

The goal is not high dialogue volume first.
The goal is meaningful branching in key moments.

## Score And Bar Logic

The user should not only see a hidden number.

The system should track:

- relationship score
- current stage threshold
- next stage threshold
- whether the next reward is world-tier gated

Suggested prototype thresholds:

- Stage 1: 15
- Stage 2: 40
- Stage 3: 75
- Stage 4: 120 plus world-tier gate
- Stage 5: 170 plus stronger world-tier gate

## Stage Rewards

Every stage should feel worth chasing.

Rewards can include:

- large material bundles
- exclusive perks
- exclusive skill boosts
- unique weapons
- unique armor pieces
- homestead upgrades
- stronger repeatable payouts

## Quest Gate Rule

Affinity rewards should not be auto-granted from score alone.

The intended structure is:

1. fill the affinity bar
2. unlock the stage quest
3. clear the dedicated quest
4. receive the stage reward package

This keeps the system from becoming a passive background checklist.

## Example Reward Direction

### Abigail

Identity:

- home stability
- recovery
- defense
- domestic stakes

Possible rewards:

- medicine-focused perk
- ranch defense upgrade
- reinforced coat or practical armor piece
- household material cache
- later-stage survival or defense weapon support

### Jack

Identity:

- lore
- clues
- archive
- relic interpretation

Possible rewards:

- clue-reading perk
- relic-oriented skill bonus
- archive weapon or charm reward
- large clue / relic cache
- late-stage myth-route unlock support

### Uncle

Identity:

- rumors
- reckless opportunity
- dirty work
- black-market routing

Possible rewards:

- outlaw or trade perk
- fence-style payout bonus
- unique weapon part or contraband gun
- salvage / cash bundles
- late-stage black-market war room options

## Repeatable NPC Integration

Affinity should not stop at family NPCs.

Repeatable quest NPCs should use affinity as a reward multiplier layer.

Simple rule:

- higher affinity = higher reward payouts
- higher affinity = better material tables
- higher affinity = occasional rare reward roll

This is especially good for:

- contract givers
- rumor brokers
- black-market buyers
- workbench-linked providers

## World Tier Relationship

Stages 4 and 5 should scale with world tier for two reasons:

- power needs a late-game gate
- loyalty should feel like campaign-level commitment, not early hub grinding

Late-stage rewards should be strong enough to matter:

- very powerful unique weapons
- large material shipments
- rare armor pieces
- elite or boss prep advantages

## System Identity In One Line

Affinity is a relationship-driven progression lane where John earns trust through gifts, dialogue, stats, skills, and dedicated quests, turning major NPCs and repeatable quest givers into meaningful sources of power, gear, and long-term frontier advantage.

## Practical Implementation Recommendation

Build affinity as an independent feature module, not as page-only UI logic.

Recommended first implementation order:

1. authored affinity definitions for Abigail, Jack, and Uncle
2. score and stage calculation
3. dialogue / gift / stat-skill contribution rules
4. dedicated quest gate definitions
5. reward package definitions
6. repeatable NPC payout scaling
7. late-stage world-tier gates

## Concrete Midgame Implementation Target

The system should be considered meaningfully underway once the following are true in code:

1. affinity grows from authored field activities instead of one hardcoded post-simulation bump
2. stages 1 and 2 can each unlock a dedicated affinity quest per family member
3. those quests can actually be marked complete and grant concrete reward bundles
4. repeatable payout bonuses read from cleared affinity reward stages, not only from raw score
5. the ledger UI shows quest state, support route, and recommended interaction
6. at least one perk branch is now gated by completed affinity work

That is the right "roughly 60%" target:
- core authored data exists
- score logic exists
- quest-gated rewards exist
- UI exposure exists
- repeatable economy tie-in exists
- later stage 3 to 5 quest scripting can still remain for future work

## Support Route Examples

The affinity page should not just show numbers.
It should frame the kind of work John is doing for each person.

- Abigail route: homefront defense, medicine stock, and practical ranch stabilization
- Jack route: archive intake, sketch review, relic sorting, and clue interpretation
- Uncle route: rumor broker work, dirty payouts, salvage leverage, and black-market routing

## Affinity-Linked Perk Direction

The first perk tie-ins should stay grounded and midgame-scaled.

- Abigail unlock direction: a homefront or recovery perk
- Jack unlock direction: an archive / clue-reading perk
- Uncle unlock direction: an outlaw payout or rumor-routing perk

These perks should require both:
- the right affinity stage
- the relevant affinity quest to be completed
