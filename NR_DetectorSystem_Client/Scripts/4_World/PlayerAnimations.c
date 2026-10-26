modded class ModItemRegisterCallbacks {
    override void RegisterOneHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior ) {
        super.RegisterOneHanded( pType, pBehavior );

        pType.AddItemInHandsProfileIK( "NR_Detector_Echo", "dz/anims/workspaces/player/player_main/props/player_main_1h_flashlight.asi", pBehavior, "dz/anims/anm/player/ik/gear/flashlight.anm" );
    };
};