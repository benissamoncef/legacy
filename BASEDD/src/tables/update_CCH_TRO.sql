-- Sectionner les tronçons des échangeurs Ollioules, Belcodène, Beausoleil
update CFG..CCH_TRO set valide=0 where autoroute=6 and numero=340
insert CFG..CCH_TRO values (344,"A52-Chateauneuf-Le-Rouge-(Noeud-A8-A52)----Belcodene",26,6,0,7500,1)
insert CFG..CCH_TRO values (346,"A52-Belcodene----La-Bouilladisse",26,6,7500,12600,1)

update CFG..CCH_TRO set valide=0 where autoroute=3 and numero=510
insert CFG..CCH_TRO values (514,"A50-Bandol----Ollioules",26,3,56100,61300,1)
insert CFG..CCH_TRO values (516,"A50-Ollioules----Six-Fours-Les-Plages",26,3,61300,63800,1)

update CFG..CCH_TRO set valide=0 where autoroute=1 and numero=290
insert CFG..CCH_TRO values (294,"A8-La-Turbie----Beausoleil",25,1,208300,211900,1)
insert CFG..CCH_TRO values (295,"A8-Beausoleil----Roquebrune-Cap-Martin",25,1,211900,214200,1)

go
