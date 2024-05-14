/*E*/
/*Fichier : @(#)xzao427sp.prc	1.1      Release : 1.1        Date : 02/03/98
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao427sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao427sp.h" */

/* static char *version = "@(#)xzao427.c	1.1 23/18/94 : xzao427" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des ZDP en aval du PMV
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAO427
go

create procedure XZAO427
   @va_numero_pmv_in		int,
   @va_touspmv_in		tinyint=0
   as 

/* 
*
* SP	XZAO427SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_numero_pmv_in		
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des  2 ZDP en aval   
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
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* PCL 	14 Jan 2005	: Creation 1.2
* PNI	21 Nov 2005	: Possibilité d'afficher TDP sur tous les PMV 1.3 DEM464
* PNI	07 avr 2006 	: Ajout de 4 PMV suppl 1.5 DEM566
* JMG	20/06/11	: ajout de 4 PMV suppl TDP A8 1.6
* LCL	20/12/11	: Modification pour tdp etendus : ajout table DTP_PMV pour association Destinataions / PMV DEM1014 1.7
* JMG	11/11/12	: filtrage numero > 0
------------------------------------------------------*/
declare
   
	/*A
	** Declaration des variables locales
	*/

	@vl_autoroute tinyint,
	@vl_sens      tinyint,
	@vl_PR        int,
	@vl_PR_destination        int,
	@vl_nom       char(15),
	@vl_numero    tinyint,
	@vl_compte    tinyint,
	@vl_nom_PMV   char(10),
	@vl_type_dest tinyint


	create table #TMP_DESTINATIONS_TDP
	(
		numero smallint,
		libelle char(15),
		type_dest tinyint
	)

	/* RESTE A FAIRE : NE RECHERCHER QUE POUR LES PMV POTENTIELLENT CANDIDATS TOUVER UNE REGLE */

	/* recherche autoroute, sens PR du PMV */
	select @vl_autoroute=autoroute, @vl_sens=sens, @vl_PR=PR,@vl_nom_PMV=nom from CFG..EQT_GEN where type=XDC_EQT_PMV and numero=@va_numero_pmv_in
   
	select @vl_numero = 0, @vl_nom=""
	select @vl_numero=isnull(R.numero, -1) , @vl_nom=R.libelle_PMV from CFG..RES_DTP R, CFG..DTP_PMV D where
	D.numero_PMV = @va_numero_pmv_in and
	D.numero_dest = R.numero and
	D.type_dest = XDC_DESTINATION_PROCHE and
	D.inhibition = 0 
	if @vl_numero > 0
	insert #TMP_DESTINATIONS_TDP values (@vl_numero, @vl_nom,XDC_DESTINATION_PROCHE)

	select @vl_numero = 0, @vl_nom=""
	select @vl_numero=isnull(R.numero, -1) , @vl_nom=R.libelle_PMV from CFG..RES_DTP R, CFG..DTP_PMV D where
	D.numero_PMV = @va_numero_pmv_in and
	D.numero_dest = R.numero and
	D.type_dest = XDC_DESTINATION_LOINTAINE and
	D.inhibition = 0 
	if @vl_numero > 0
	insert #TMP_DESTINATIONS_TDP values (@vl_numero, @vl_nom,XDC_DESTINATION_LOINTAINE)

	select @vl_numero = 0, @vl_nom=""
	select @vl_numero=isnull(R.numero, -1) , @vl_nom=R.libelle_PMV from CFG..RES_DTP R, CFG..DTP_PMV D where
	D.numero_PMV = @va_numero_pmv_in and
	D.numero_dest = R.numero and
	D.type_dest = XDC_DESTINATION_NOEUD and
	D.inhibition = 0 
	if @vl_numero > 0
	insert #TMP_DESTINATIONS_TDP values (@vl_numero, @vl_nom,XDC_DESTINATION_NOEUD)
	

   /* on retourne les resultats */
   select numero,convert(char(10),libelle) from #TMP_DESTINATIONS_TDP order by type_dest
   return XDC_OK
         
go  
