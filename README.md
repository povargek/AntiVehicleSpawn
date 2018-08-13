# AntiVehicleSpawn
Fix no controled vehicle spawn in SA:MP (OnVehicleDeath)

DEPRECATED, USE A PAWN RAKNET
=========================
https://github.com/urShadow/Pawn.RakNet/wiki/AntiVehicleSpawn


EXAMPLE
-----------
        public OnVehicleRequestDeath(vehicleid, killerid)
        {
            new Float:Health;
            GetVehicleHealth(vehicleid, Health);
        
            if(Health > 300.0)
            {
                return 0;
            }
        
            return 1;
        }

REQUIREMENTS
------------

Plugin Tested ONLY on SA:MP Server 0.3.7 R2-1, operability is not guaranteed on other versions
		
RECOMMENDATION
-----------

OnVehicleDeath don't call, if return false in OnVehicleRequestDeath.
If OnVehicleRequestDeath not forwarded in gamemode, OnVehicleDeath call always

WARNING! OnVehicleRequestDeath Must be forwarded ONLY in Main GameMode (NOT IN FILTERSCIPT!)
If OnVehicleRequestDeath return true, will be be spawned, and OnVehicleDeath it is called in Gamemode and Filterscript's,
returning false in OnVehicleRequestDeath, stop a spawn vehicle, and OnVehicleDeath don't be called

CREDITS
-----------

SA:MP Server 0.3.7 Structs from YSF by kurta999 (https://github.com/kurta999/YSF)
And all other copyrights are listed in the files of source code

Sorry for my bad English
