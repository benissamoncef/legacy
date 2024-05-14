/*E*/
/*Fichier : @(#)xzao434sp.prc	1.3      Release : 1.3        Date : 02/14/97
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao434sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/
/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des zones origines et destinations pour tdp a envoyer a RTFM
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
* SP	XZAO434_Liste_origines_destinations
*
* PARAMETRES EN ENTREE :
* si va_appelant = 1 alors insert
*                  2       supp
*                  3       modif
*		   0       select * from TFM_TDP
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* CHI	01/07/2005: Creation v1.1
* LCL	24/01/2012: Temps de parcours etendus DEM1014 v1.2
* PNI	26/06/2012: ordonne par sens PR v1.3
* PNI	27/11/2012 : ordonne par PR suivant le sens v1.4
------------------------------------------------------*/
    
use PRC
go

if exists (select * from sysobjects where name = 'XZAO434' and type = 'P')
	drop procedure XZAO434
go

create procedure XZAO434
   @va_appelant         int,
   @va_autoroute	int,
   @va_sens		int,	
   @va_zone_origine	char(30),
   @va_zone_destination char(30)
   as 
   declare
		@vl_numero           int
   if   @va_zone_origine =null or 
	@va_zone_destination = null or
	@va_sens = null
   return XDC_ARG_INV

   if @va_appelant = 0
   begin
	create table #TMP_DTP
	( nom char(4) null,
	  sens tinyint null,
	  origine char(30) null,
	  destination char(30) null,
	  numero tinyint null)
	/* A8 */
	insert into #TMP_DTP select AUT.nom, TFM.sens, TFM.origine, TFM.destination , AUT.numero
	from EXP..TFM_TDP TFM,
	     CFG..RES_AUT AUT, CFG..RES_DTP D
	where AUT.numero = TFM.autoroute and TFM.sens=D.sens and TFM.autoroute=D.autoroute and libelle_TFM=TFM.origine
	and D.sens=1 and D.autoroute=1
   	order by D.PR
	insert into #TMP_DTP select AUT.nom, TFM.sens, TFM.origine, TFM.destination , AUT.numero
	from EXP..TFM_TDP TFM,
	     CFG..RES_AUT AUT, CFG..RES_DTP D
	where AUT.numero = TFM.autoroute and TFM.sens=D.sens and TFM.autoroute=D.autoroute and libelle_TFM=TFM.origine
	and D.sens=2 and D.autoroute=1
   	order by D.PR desc

	/* A57 */
	insert into #TMP_DTP select AUT.nom, TFM.sens, TFM.origine, TFM.destination , AUT.numero
	from EXP..TFM_TDP TFM,
	     CFG..RES_AUT AUT, CFG..RES_DTP D
	where AUT.numero = TFM.autoroute and TFM.sens=D.sens and TFM.autoroute=D.autoroute and libelle_TFM=TFM.origine
	and D.sens=1 and D.autoroute=8
   	order by D.PR desc
	insert into #TMP_DTP select AUT.nom, TFM.sens, TFM.origine, TFM.destination , AUT.numero
	from EXP..TFM_TDP TFM,
	     CFG..RES_AUT AUT, CFG..RES_DTP D
	where AUT.numero = TFM.autoroute and TFM.sens=D.sens and TFM.autoroute=D.autoroute and libelle_TFM=TFM.origine
	and D.sens=2 and D.autoroute=8
   	order by D.PR 

	/* A50 */
	insert into #TMP_DTP select AUT.nom, TFM.sens, TFM.origine, TFM.destination , AUT.numero
	from EXP..TFM_TDP TFM,
	     CFG..RES_AUT AUT, CFG..RES_DTP D
	where AUT.numero = TFM.autoroute and TFM.sens=D.sens and TFM.autoroute=D.autoroute and libelle_TFM=TFM.origine
	and D.sens=2 and D.autoroute=3
   	order by D.PR desc
	insert into #TMP_DTP select AUT.nom, TFM.sens, TFM.origine, TFM.destination , AUT.numero
	from EXP..TFM_TDP TFM,
	     CFG..RES_AUT AUT, CFG..RES_DTP D
	where AUT.numero = TFM.autoroute and TFM.sens=D.sens and TFM.autoroute=D.autoroute and libelle_TFM=TFM.origine
	and D.sens=1 and D.autoroute=3
   	order by D.PR
	
	/* A52 */
	insert into #TMP_DTP select AUT.nom, TFM.sens, TFM.origine, TFM.destination , AUT.numero
	from EXP..TFM_TDP TFM,
	     CFG..RES_AUT AUT, CFG..RES_DTP D
	where AUT.numero = TFM.autoroute and TFM.sens=D.sens and TFM.autoroute=D.autoroute and libelle_TFM=TFM.origine
	and D.sens=1 and D.autoroute=6
   	order by D.PR
	insert into #TMP_DTP select AUT.nom, TFM.sens, TFM.origine, TFM.destination , AUT.numero
	from EXP..TFM_TDP TFM,
	     CFG..RES_AUT AUT, CFG..RES_DTP D
	where AUT.numero = TFM.autoroute and TFM.sens=D.sens and TFM.autoroute=D.autoroute and libelle_TFM=TFM.origine
	and D.sens=2 and D.autoroute=6
   	order by D.PR desc

	/* A51 */
	insert into #TMP_DTP select AUT.nom, TFM.sens, TFM.origine, TFM.destination , AUT.numero
	from EXP..TFM_TDP TFM,
	     CFG..RES_AUT AUT, CFG..RES_DTP D
	where AUT.numero = TFM.autoroute and TFM.sens=D.sens and TFM.autoroute=D.autoroute and libelle_TFM=TFM.origine
	and D.sens=1 and D.autoroute=5
   	order by D.PR
	insert into #TMP_DTP select AUT.nom, TFM.sens, TFM.origine, TFM.destination , AUT.numero
	from EXP..TFM_TDP TFM,
	     CFG..RES_AUT AUT, CFG..RES_DTP D
	where AUT.numero = TFM.autoroute and TFM.sens=D.sens and TFM.autoroute=D.autoroute and libelle_TFM=TFM.origine
	and D.sens=2 and D.autoroute=5
   	order by D.PR desc

	select * from #TMP_DTP
   end
   else
   if @va_appelant = 1
   /*insertion*/
   begin
	insert into EXP..TFM_TDP
	values(@va_sens, @va_zone_origine, @va_zone_destination,@va_autoroute)
   end
   else
   if @va_appelant = 2
   /*suppression*/
   begin
   	delete from EXP..TFM_TDP
	where   sens=@va_sens and
		origine=@va_zone_origine and
		destination=@va_zone_destination
   end
   return XDC_OK
go  
