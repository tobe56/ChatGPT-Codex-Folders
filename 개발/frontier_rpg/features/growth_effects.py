from __future__ import annotations

from ..models import PlayerProfile


def _has_perk(player: PlayerProfile, perk_id: str) -> bool:
    return perk_id in player.unlocked_perks


def _item_total(player: PlayerProfile, item_name: str) -> int:
    return player.inventory.get(item_name, 0) + player.stash_inventory.get(item_name, 0)


def build_growth_effects(player: PlayerProfile) -> dict[str, int]:
    combat_damage = ((player.stats.strength - 2) * 4) + ((player.stats.deadeye - 2) * 3)
    if _has_perk(player, "gunslinger.quick_draw"):
        combat_damage += 4
    if _has_perk(player, "gunslinger.iron_wrist"):
        combat_damage += 6
    if _has_perk(player, "survivor.ranch_resolve"):
        combat_damage += 4

    boss_damage = (_has_perk(player, "relic_hunter.ash_mark") * 12) + (_has_perk(player, "relic_hunter.grave_luck") * 6)
    damage_resist = ((player.stats.grit - 2) * 5) + ((_has_perk(player, "survivor.thick_skin")) * 10)
    if _has_perk(player, "survivor.ranch_resolve"):
        damage_resist += 6
    if _item_total(player, "Saint Medal") > 0:
        damage_resist += 4

    loot_yield = ((player.stats.cunning - 2) * 4) + (player.lifestyle.salvaging * 4) + (player.lifestyle.hunting * 2)
    if _has_perk(player, "outlaw.fast_pockets"):
        loot_yield += 8
    if _has_perk(player, "outlaw.greased_palm"):
        loot_yield += 6
    if _has_perk(player, "relic_hunter.grave_luck"):
        loot_yield += 10

    travel_stamina = ((player.stats.survival - 2) * 4) + (player.lifestyle.horse_handling * 4)
    if _has_perk(player, "frontiersman.long_trail"):
        travel_stamina += 10
    if _item_total(player, "Scout Saddle Token") > 0:
        travel_stamina += 6

    craft_cost_reduction = (player.lifestyle.gunsmithing * 3) + (player.lifestyle.homestead * 2)
    if _has_perk(player, "frontiersman.camp_mechanic"):
        craft_cost_reduction += 6
    if _has_perk(player, "survivor.patch_job"):
        craft_cost_reduction += 3

    medicine_efficiency = (player.lifestyle.field_medicine * 5) + (_has_perk(player, "survivor.household_reserve") * 8)
    drop_quality = (player.lifestyle.hunting * 3) + (player.lifestyle.trapping * 3) + (_has_perk(player, "relic_hunter.grave_luck") * 8)
    mounted_control = (player.lifestyle.horse_handling * 5) + (_has_perk(player, "frontiersman.long_trail") * 6)

    return {
        "combat_damage_bonus_percent": max(0, combat_damage),
        "boss_damage_bonus_percent": max(0, boss_damage),
        "damage_resist_percent": max(0, damage_resist),
        "loot_yield_bonus_percent": max(0, loot_yield),
        "travel_stamina_bonus_percent": max(0, travel_stamina),
        "craft_cost_reduction_percent": max(0, craft_cost_reduction),
        "medicine_efficiency_percent": max(0, medicine_efficiency),
        "drop_quality_bonus_percent": max(0, drop_quality),
        "mounted_control_bonus_percent": max(0, mounted_control),
    }
