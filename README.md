# Custom-Name-Tags-SA-MP

🎯 Main Features

1. Underscore Removal
Player names that were previously displayed as Jackson_Whayn will now appear as Jackson Whayn (with a space).
This makes names look more natural and easier to read.
The Nametag_RemoveUnderscore() function automatically processes player names.

2. Stable Positioning
Uses an Attached 3D Text Label that is attached directly to the player.
It will not drop or shift when another player gets close (a common issue with standard nametags).
The height offset can be customized via NAMETAG_HEIGHT_OFFSET.
Always follows the player’s position perfectly.

3. Damage Visual Feedback
Automatically changes color to red when a player takes damage.
Returns to white after a few milliseconds (default: 500 ms).
Provides clear visual feedback during combat.
The flash duration can be configured via DAMAGE_FLASH_TIME.

4. Automatic Damage Detection
Uses a timer task UpdateNameTag[1000] that runs every 1 second.
Compares current health and armor with previous values.
If there is a decrease, the player has taken damage.
Does not require the OnPlayerTakeDamage callback.

5. Performance Optimization
Uses YSI y_timers for efficient task management.
Loops only through online players (GetPlayerPoolSize()).
Skips players who have not spawned yet (PLAYER_STATE_NONE).
Minimal overhead on server performance.
