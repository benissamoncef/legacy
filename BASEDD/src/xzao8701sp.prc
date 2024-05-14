/*E*/
/* Fichier : $Id: xzao8701sp.prc,v 1.2 2018/11/05 17:40:33 devgfi Exp $        $Revision: 1.2 $        $Date: 2018/11/05 17:40:33 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao8701sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABK	      25/08/23        : Creation (DEM/483)
------------------------------------------------------*/

/* fichiers inclus */

/* static char *version = "@(#)xzao8701sp.prc	1.2 1.2 11/19/07: xzao8701" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des PIC pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO8701' and type = 'P')
	drop procedure XZAO8701
go


create procedure XZAO8701
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO8701SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
* XDY_Machine	va_NomMachine_in		
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des PIC de la Machine 
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
* Retourne les parametres numero de PIC, nom du ST, numero de liaison, adresse rgs
* numero de type de PIC
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
	/* Declaration des variables locales */
	@vl_Machine		smallint
 
	/* Test des parametres d'entree : Si un des parametres d'entree est egal a null alors XDC_ARG_INV */
	if @va_Basedd_in = null 
		return XDC_ARG_INV
	else   
	begin

		if rtrim (@va_NomMachine_in) = NULL select @va_NomMachine_in=NULL

		/* Travail dans la base CFG */   
		if @va_Basedd_in = XDC_BASE_CFG
		begin   
			/* Recherche de la liste des PIC associes a plusieurs serveurs */
			select distinct PIC.numero,
							PIC.liaison_serie,
							PIC.rgs,
							GEN.nom, 
							PIC.adresse_ip,
							PIC.port,
							GEN2.site_gestion
				from	CFG..EQT_GEN GEN, 
						CFG..EQT_GEN GEN2, 
						CFG..EQT_PIC PIC
					where GEN.type         = XDC_EQT_SRV
						and GEN.type_maitre  = XDC_MAI_SRV
						and GEN2.type        = XDC_EQT_PIC
						and GEN2.maitre      = GEN.numero
						and GEN2.type_maitre = XDC_MAI_PIC
						and GEN2.eqt_supprime = XDC_FAUX
						and PIC.numero       = GEN2.numero
							order by PIC.numero, GEN.nom
		end
      
#ifdef CI
		/* Travail dans la base CFT */   
		else
		begin
			/* Recherche de la liste des PIC associes a plusieurs serveurs */
			select distinct PIC.numero,
								PIC.liaison_serie,
								PIC.rgs,
								GEN.nom, 
								PIC.adresse_ip,
								PIC.port,
								GEN2.site_gestion 
				from	CFT..EQT_GEN GEN, 
						CFT..EQT_GEN GEN2, 
						CFT..EQT_PIC PIC
					where GEN.type         = XDC_EQT_SRV
						and GEN.type_maitre  = XDC_MAI_SRV
						and GEN2.type        = XDC_EQT_PIC
						and GEN2.maitre      = GEN.numero
						and GEN2.type_maitre = XDC_MAI_PIC
						and GEN2.eqt_supprime = XDC_FAUX
						and PIC.numero       = GEN2.numero
							order by GEN.nom, PIC.numero   
		end
#endif
	end
	return XDC_OK
go
