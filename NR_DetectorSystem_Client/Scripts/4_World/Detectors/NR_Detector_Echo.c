class NR_Detector_Echo extends ItemBase {
    private EntityAI pOwner;

    // Состояние детектора
    bool m_IsDetectorOn = false;
    float m_SignalStrength = 0.0;
    
    // Звук и анимация
    private EffectSound m_BeepSound;
    private float m_LastBeepTime;
    private float m_BeepInterval = 0.0;
    private bool m_IsLampOn = false;
    
    // Таймеры
    private ref Timer m_LampOffTimer;
    
    void NR_Detector_Echo() {
        GetCompEM().SwitchOff();
        SwitchOn( false );

        #ifndef SERVER
        m_LampOffTimer = new Timer( CALL_CATEGORY_GUI );
        #endif
    };

    void ~NR_Detector_Echo() {
        StopBeepSound();
        StopVisualUpdates();
        
        if ( m_LampOffTimer && m_LampOffTimer.IsRunning() )
            m_LampOffTimer.Stop();
    };

    override void SetActions() {
        super.SetActions();
        AddAction( ActionTurnOnWhileInHands );
        AddAction( ActionTurnOffWhileInHands );
    };

    override void OnSwitchOn() {
        if ( !GetCompEM().CanWork() )
            GetCompEM().SwitchOff();
    };

    override void OnWorkStart() {
        super.OnWorkStart();
        SwitchOn( true );

        #ifndef SERVER
        pOwner = GetHierarchyParent();

        if ( !pOwner ) return;
        if ( !pOwner.IsPlayer() ) return;

        ToggleDetector( true );
        #endif
    };

    override void OnWorkStop() {
        super.OnWorkStop();

        GetCompEM().SwitchOff();
        SwitchOn( false );

        ToggleDetector( false );
        StopVisualUpdates();
    };

    void StopVisualUpdates() {
        SetLampState( false );
        m_SignalStrength = 0.0;
    };

    void ToggleDetector( bool bToggle )  {
        m_IsDetectorOn = bToggle;
        
        if ( bToggle ) SetAnimationPhase( "cover", 1.0 );
        else SetAnimationPhase( "cover", 0.0 );
        
        SetLampState( false );
        StopBeepSound();
        m_SignalStrength = 0.0;
    };

    void SetLampState( bool bState ) {
        if ( m_IsLampOn == bState ) return;
        
        m_IsLampOn = bState;
        
        if ( bState ) {
            SetAnimationPhase( "lampOff", 0.0 );
            SetAnimationPhase( "lampOn", 1.0 );
        }
        else {
            SetAnimationPhase( "lampOff", 1.0 );
            SetAnimationPhase( "lampOn", 0.0 );
        };
    };
    
    void TurnOffLampAfterDelay() {
        SetLampState( false );
    };

    void NR_DetectorEcho_SetSignal( float flStrength ) {
        if ( m_IsDetectorOn )
            m_SignalStrength = flStrength;
    };

    override void EEItemLocationChanged( notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc ) {
        super.EEItemLocationChanged( oldLoc, newLoc );
        
        PlayerBase old_player = null;

        if ( oldLoc.GetParent() )
            old_player = PlayerBase.Cast( oldLoc.GetParent().GetHierarchyRootPlayer() );
        
        if ( old_player && oldLoc.GetType() == InventoryLocationType.HANDS ) {
            GetCompEM().SwitchOff();
            SwitchOn( false );

            ToggleDetector( false );
            StopVisualUpdates();
        };
    };

    override void OnWork( float consumed_energy ) {
        #ifndef SERVER
        if ( m_IsDetectorOn ) {
            private PlayerBase pPlayer = PlayerBase.Cast( pOwner );

            if ( pPlayer && pPlayer.IsAlive() ) {
                float currentTime = GetGame().GetTime(); // миллисекунды
                
                // Только если есть сигнал
                if ( m_SignalStrength > 0.01 ) {
                    // Интервалы: 2.0 сек (слабый сигнал) - 0.2 сек (сильный сигнал)
                    float minInterval = 0.15;
                    float maxInterval = 2.0;
                    
                    m_BeepInterval = maxInterval - ( m_SignalStrength * ( maxInterval - minInterval ) );
                    
                    // Умножаем на 1000 для перевода в миллисекунды
                    if ( currentTime - m_LastBeepTime >= m_BeepInterval * 1000 ) {
                        m_LastBeepTime = currentTime;
                        SetLampState( true );
                        PlayBeepSound();
                        
                        if ( m_LampOffTimer ) {
                            m_LampOffTimer.Stop();
                            m_LampOffTimer.Run( 0.145, this, "TurnOffLampAfterDelay", NULL, false );
                        };
                    };
                }
                else {
                    // Если сигнала нет — ничего не делаем, детектор молчит
                    if ( m_IsLampOn )
                        SetLampState( false );
                };
            };
        };
        #endif
    };

    void PlayBeepSound() {
        StopBeepSound();
        m_BeepSound = SEffectManager.PlaySoundOnObject( "EchoBeepSoundSet", this, 0, false );
    };

    void StopBeepSound() {
        if ( m_BeepSound ) {
            SEffectManager.DestroyEffect( m_BeepSound );
            m_BeepSound = null;
        };
    };
    
    override void OnRPC( PlayerIdentity sender, int rpc_type, ParamsReadContext ctx ) {
        super.OnRPC( sender, rpc_type, ctx );
        
        if ( rpc_type == NR_eDetectorRPCs.RPC_DETECTOR_SIGNAL ) {
            float flStrength;
            
            if ( ctx.Read( flStrength ) )
                NR_DetectorEcho_SetSignal( flStrength );
        };
    };
};