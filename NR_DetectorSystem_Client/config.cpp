class CfgPatches {
	class NR_DetectorSystem_Client {
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = { "DZ_Data", "DZ_Scripts", "DZ_Gear_Consumables" };
	};
};

class CfgMods {
	class NR_DetectorSystem_Client {
		name = "Detector System | Client Side"; 								// name
		picture = "";			// picture in expanded description
		logoSmall = "";	// icon next to mod name when description is not expanded
		logo = "";				// logo below game menu
		logoOver = "";		// on mouse hover over logo
		tooltip = "";							// tool tip on mouse hover
		overview = "Discord: nimroK"; 						// overview
		action = "https://steamcommunity.com/id/n1mroK/";					// link
		author = "nimroK";								// author
		version = "1.0";								// version
		type = "mod";
		dependencies[] = { "Game", "World" };
		
		class defs {
			class gameScriptModule {
				value = "";
				files[] = { "NR_DetectorSystem_Client/Scripts/3_Game" };
			};
			
			class worldScriptModule {
				value = "";
				files[] = { "NR_DetectorSystem_Client/Scripts/4_World" };
			};
		};
	};
};

class CfgVehicles {
	class Inventory_Base;

    class NR_Detector_Echo: Inventory_Base
    {
        scope = 2;
        displayName = "Детектор «Отклик»";
        descriptionShort = "Начальный детектор для поиска артефактов.";
		model = "NR_DetectorSystem_Client\Models\Echo\Echo.p3d";
		inventorySlot[]={"NR_Detector"};
		itemSize[] = {1,2};
		weight = 500;
		repairableWithKits[] = {7};
		repairCosts[] = {10};
		attachments[] = {"BatteryD"};
		itemBehaviour = 1;
        varEnergyMax = 100;
        varEnergyUsagePerSecond = 0.1;
		hiddenSelections[] = {"body", "lampoff", "lampon", "top"};
        hiddenSelectionsTextures[] = {"NR_DetectorSystem_Client\Models\Echo\data\Echo_co.paa", "NR_DetectorSystem_Client\Models\Echo\data\Echo_co.paa", "NR_DetectorSystem_Client\Models\Echo\data\Echo_co.paa", "NR_DetectorSystem_Client\Models\Echo\data\Echo_co.paa"};
		
		class EnergyManager {
			hasIcon = 1;
			autoSwitchOff = 1;
			autoSwitchOffWhenInCargo = 1;
			energyUsagePerSecond = 0.001;
			plugType = 1;
			attachmentAction = 1;
			updateInterval = 0.15;
		};
		
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1,{""}},{0.7,{""}},{0.5,{""}},{0.3,{""}},{0,{""}}};
				};
			};
		};
		
		class AnimationSources
		{
			class lampOff
			{
				source = "user";
				animPeriod = 0.001;
				initPhase = 1;
			};
			class lampOn
			{
				source = "user";
				animPeriod = 0.001;
				initPhase = 0;
			};
			class cover
			{
				source = "user";
				animPeriod = 0.5;
				initPhase = 0;
			};
		};
		
		class AnimEvents
		{
			class SoundWeapon
			{
				class compass_open
				{
					soundSet = "compass_open_SoundSet";
					id = 204;
				};
				class compass_close
				{
					soundSet = "compass_close_SoundSet";
					id = 205;
				};
			};
		};
    };
};

class CfgSoundShaders
{
	class SpawnArtSoundShader
	{
		samples[] = {{"NR_DetectorSystem_Client\Sounds\artifact",1}};
		volume = 1;
		range = 5;
	};
	class EchoBeepSoundShader
	{
		samples[] = {{"NR_DetectorSystem_Client\Sounds\echo",1}};
		volume = 1;
		range = 5;
	};
};

class CfgSoundSets
{
	class SpawnArtSoundSet
	{
		sound3DProcessingType = "infected3DProcessingType";
		volumeCurve = "infectedAttenuationCurve";
		spatial = 1;
		doppler = 0;
		soundShaders[] = {"SpawnArtSoundShader"};
	};
	class EchoBeepSoundSet
	{
		sound3DProcessingType = "infected3DProcessingType";
		volumeCurve = "infectedAttenuationCurve";
		spatial = 1;
		doppler = 0;
		soundShaders[] = {"EchoBeepSoundShader"};
	};
};