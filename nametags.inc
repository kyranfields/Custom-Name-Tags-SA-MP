/*
    Author: Kyran
    Version: 1.0
    
    Features:
    - Removes underscore from player names
    - Stable position when approached (attached to player)
    - Red color flash when taking damage
    - Optimized with task timer
    - Easy to integrate as include file
    
    Requirements:
    - Streamer Plugin (by Incognito)
    - YSI Library (y_timers)
    
    Installation:
    1. Put this file in pawno/include folder
    2. Add to your gamemode: #include <nametags>
    3. Done!
*/

#if defined _nametags_included
    #endinput
#endif
#define _nametags_included

// Dependencies
#if !defined _samp_included
    #error "Please include a_samp before nametags"
#endif

#if !defined STREAMER_TYPE_3D_TEXT_LABEL
    #tryinclude <streamer>
#endif

#if !defined _YSI_y_timers
    #tryinclude <YSI_Coding\y_timers>
#endif

// CONFIGURATION

#if !defined NAMETAG_DRAW_DISTANCE
    #define NAMETAG_DRAW_DISTANCE   25.0
#endif

#if !defined NAMETAG_COLOR_DEFAULT
    #define NAMETAG_COLOR_DEFAULT   0xFFFFFFFF  // Putih
#endif

#if !defined NAMETAG_COLOR_DAMAGE
    #define NAMETAG_COLOR_DAMAGE    0xFF0000FF  // Merah
#endif

#if !defined NAMETAG_HEIGHT_OFFSET
    #define NAMETAG_HEIGHT_OFFSET   0.75        // Offset tinggi dari kepala
#endif

#if !defined DAMAGE_FLASH_TIME
    #define DAMAGE_FLASH_TIME       500         // Durasi flash merah (ms)
#endif

#if !defined NAMETAG_UPDATE_RATE
    #define NAMETAG_UPDATE_RATE     1000        // Update rate (ms)
#endif

// VARIABLES

enum E_NAMETAG_DATA 
{
    Text3D:NT_Label,
    bool:NT_Damaged,
    Float:NT_LastHealth,
    Float:NT_LastArmour
}

static PlayerNametagData[MAX_PLAYERS][E_NAMETAG_DATA];

forward NT_OnGameModeInit();
forward NT_OnPlayerConnect(playerid);
forward NT_OnPlayerDisconnect(playerid, reason);
forward NT_OnPlayerSpawn(playerid);

public OnGameModeInit() {
    ShowPlayerMarkers(PLAYER_MARKERS_MODE_OFF);
    
    #if defined NT_OnGameModeInit
        return NT_OnGameModeInit();
    #else
        return 1;
    #endif
}

public OnPlayerConnect(playerid) {
    PlayerNametagData[playerid][NT_Damaged] = false;
    PlayerNametagData[playerid][NT_LastHealth] = 100.0;
    PlayerNametagData[playerid][NT_LastArmour] = 0.0;
    PlayerNametagData[playerid][NT_Label] = Text3D:INVALID_3DTEXT_ID;
    
    Nametag_Create(playerid);
    
    #if defined NT_OnPlayerConnect
        return NT_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}

public OnPlayerDisconnect(playerid, reason) {
    Nametag_Destroy(playerid);
    
    #if defined NT_OnPlayerDisconnect
        return NT_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}

public OnPlayerSpawn(playerid) {
    Nametag_Update(playerid);
    PlayerNametagData[playerid][NT_LastHealth] = 100.0;
    PlayerNametagData[playerid][NT_LastArmour] = 0.0;
    
    #if defined NT_OnPlayerSpawn
        return NT_OnPlayerSpawn(playerid);
    #else
        return 1;
    #endif
}

task UpdateNameTag[NAMETAG_UPDATE_RATE]() {
    new Float:health, Float:armour;
    
    for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++) {
        if(!IsPlayerConnected(i)) continue;
        if(GetPlayerState(i) == PLAYER_STATE_NONE) continue;
        
        GetPlayerHealth(i, health);
        GetPlayerArmour(i, armour);
        
        // Deteksi damage
        if(health < PlayerNametagData[i][NT_LastHealth] || armour < PlayerNametagData[i][NT_LastArmour]) {
            Nametag_OnDamage(i);
        }
        
        PlayerNametagData[i][NT_LastHealth] = health;
        PlayerNametagData[i][NT_LastArmour] = armour;
    }
}

task ResetNametagColor[DAMAGE_FLASH_TIME]() {
    for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++) {
        if(!IsPlayerConnected(i)) continue;
        
        if(PlayerNametagData[i][NT_Damaged]) {
            PlayerNametagData[i][NT_Damaged] = false;
            Nametag_SetColor(i, NAMETAG_COLOR_DEFAULT);
        }
    }
}

stock Nametag_Create(playerid) {
    if(!IsPlayerConnected(playerid)) return 0;
    
    new name[MAX_PLAYER_NAME], cleanName[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    
    Nametag_RemoveUnderscore(name, cleanName, sizeof(cleanName));
    
    if(IsValidDynamic3DTextLabel(PlayerNametagData[playerid][NT_Label])) {
        DestroyDynamic3DTextLabel(PlayerNametagData[playerid][NT_Label]);
    }
    
    PlayerNametagData[playerid][NT_Label] = CreateDynamic3DTextLabel(
        cleanName,
        NAMETAG_COLOR_DEFAULT,
        0.0, 0.0, NAMETAG_HEIGHT_OFFSET,
        NAMETAG_DRAW_DISTANCE,
        playerid,
        INVALID_VEHICLE_ID,
        0,
        -1, -1, -1,
        NAMETAG_DRAW_DISTANCE
    );
    
    return 1;
}

stock Nametag_Destroy(playerid) {
    if(IsValidDynamic3DTextLabel(PlayerNametagData[playerid][NT_Label])) {
        DestroyDynamic3DTextLabel(PlayerNametagData[playerid][NT_Label]);
        PlayerNametagData[playerid][NT_Label] = Text3D:INVALID_3DTEXT_ID;
        return 1;
    }
    return 0;
}

stock Nametag_Update(playerid) {
    if(!IsPlayerConnected(playerid)) return 0;
    if(!IsValidDynamic3DTextLabel(PlayerNametagData[playerid][NT_Label])) return 0;
    
    new name[MAX_PLAYER_NAME], cleanName[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    Nametag_RemoveUnderscore(name, cleanName, sizeof(cleanName));
    
    UpdateDynamic3DTextLabelText(
        PlayerNametagData[playerid][NT_Label],
        NAMETAG_COLOR_DEFAULT,
        cleanName
    );
    
    return 1;
}

stock Nametag_SetColor(playerid, color) {
    if(!IsPlayerConnected(playerid)) return 0;
    if(!IsValidDynamic3DTextLabel(PlayerNametagData[playerid][NT_Label])) return 0;
    
    new name[MAX_PLAYER_NAME], cleanName[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    Nametag_RemoveUnderscore(name, cleanName, sizeof(cleanName));
    
    UpdateDynamic3DTextLabelText(
        PlayerNametagData[playerid][NT_Label],
        color,
        cleanName
    );
    
    return 1;
}

stock Nametag_OnDamage(playerid) {
    PlayerNametagData[playerid][NT_Damaged] = true;
    Nametag_SetColor(playerid, NAMETAG_COLOR_DAMAGE);
    return 1;
}

stock Nametag_RemoveUnderscore(const input[], output[], maxlength = sizeof(output)) {
    new pos = 0;
    for(new i = 0; input[i] != EOS && pos < maxlength - 1; i++) {
        output[pos++] = (input[i] == '_') ? ' ' : input[i];
    }
    output[pos] = EOS;
    return 1;
}

stock Nametag_SetDrawDistance(playerid, Float:distance) {
    if(!IsPlayerConnected(playerid)) return 0;
    if(!IsValidDynamic3DTextLabel(PlayerNametagData[playerid][NT_Label])) return 0;
    
    Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, PlayerNametagData[playerid][NT_Label], E_STREAMER_STREAM_DISTANCE, distance);
    return 1;
}

stock Text3D:Nametag_GetLabel(playerid) {
    if(!IsPlayerConnected(playerid)) return Text3D:INVALID_3DTEXT_ID;
    return PlayerNametagData[playerid][NT_Label];
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit NT_OnGameModeInit
#if defined NT_OnGameModeInit
    forward NT_OnGameModeInit();
#endif

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect NT_OnPlayerConnect
#if defined NT_OnPlayerConnect
    forward NT_OnPlayerConnect(playerid);
#endif

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect NT_OnPlayerDisconnect
#if defined NT_OnPlayerDisconnect
    forward NT_OnPlayerDisconnect(playerid, reason);
#endif

#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn NT_OnPlayerSpawn
#if defined NT_OnPlayerSpawn
    forward NT_OnPlayerSpawn(playerid);
#endif