class CfgPatches {
	class NR_AnomalySystem_Client {
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = { "DZ_Data" };
	};
};

class CfgMods {
	class NR_AnomalySystem_Client {
		name = "Anomaly System | Client Side"; 								// name
		overview = "Discord: n1mroK"; 						// overview
		action = "https://steamcommunity.com/id/n1mroK/";					// link
		author = "nimroK";								// author
		type = "mod";
		dependencies[] = { "Game", "World" };
		
		class defs {
			class gameScriptModule {
				value = "";
				files[] = { "NR_AnomalySystem_Client/Scripts/3_Game" };
			};
			
			class worldScriptModule {
				value = "";
				files[] = { "NR_AnomalySystem_Client/Scripts/4_World" };
			};
		};
	};
};

class CfgVehicles {
	class HouseNoDestruct;
	class SFP_st_bolt01;

	/* ~ [ Bolt ] ~ */
	class NR_Anomaly_Bolt: SFP_st_bolt01 {
		scope = 2;
		displayName = "Болт";
		descriptionShort = "Если кинуть в аномалию, может её разрядить.";
		inventorySlot[]={"NR_Bolt"};
	};

	/* ~ [ Base ] ~ */
	class NR_Anomaly_Base: HouseNoDestruct {
		scope = 0;
		model = "NR_AnomalySystem_Client\Models\Anomaly_Base.p3d";
	};
	
	/* ~ [ Electra ] ~ */
	class NR_Anomaly_ElectraBase: NR_Anomaly_Base {};
	class NR_Anomaly_ElectraIdle: NR_Anomaly_Base {};
	class NR_Anomaly_ElectraActive: NR_Anomaly_Base {};
};

class CfgSounds {
	class default {
		name = "";
		titles[] = {};
	};
	
	/* ~ [ Electra ] ~ */
	class Electra_Idle: default {
		sound[] = { "NR_AnomalySystem_Client\Sounds\Electra\Idle", 3, 1, 30 };
	};
	
	class Electra_Active: default {
		sound[] = { "NR_AnomalySystem_Client\Sounds\Electra\Active", 3, 1, 50 };
	};
};