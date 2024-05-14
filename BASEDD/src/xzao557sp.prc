/*E*/
/* Fichier : $Id: xzao557sp.prc,v 1.6 2018/07/11 10:00:33 gesconf Exp $        Release : $Revision: 1.6 $        Date : $Date: 2018/07/11 10:00:33 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao557sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		13/01/12 : Creation (DEM/1016)
* JMG		23/07/12 : correction requete 1.2
* JMG		23/01/13 : correction requete sens 1.3
* LCL		05/07/17 : modif PRA 1.4
* PNI		10/07/18 : correctif select du serveur 1.5
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao557sp.h" */

/* static char *version = "@(#)xzao557.c	1.1 23/18/94 : xzao557" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO557' and type = 'P')
	drop procedure XZAO557
go

create procedure XZAO557
   @va_Basedd_in             char(3),
   @va_NumSecteur_in 		int
   as 

/* 
*
* SP    XZAO555SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*  					va_NumSecteur_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out
*
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Affichage des donnees de la table ZON_REG
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012 : Creation (DEM/1016)
* GGY	26/09/23 	: Ajout district CA (DEM483)
* GGY	17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
	@vl_Site 	char(10)
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
      
    if @va_Basedd_in             = null 
        
		return XDC_ARG_INV
   
    else   
    begin
      
		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin

            /*A
            **  Travail dans la base CFG
            */   
      
				if @va_Basedd_in = XDC_BASE_CFG
            
					begin
                        /*A
                        ** Lecture des donnees dans ZON_REG
                        */

						select distinct
						ZON.numero, 
						ZON.nom, 
						ZON.autoroute, 
						ZON.sens,
						debut, 
						fin,
						vitesse_amont, 
						inhibition,
						mode_horaire,
						GEN.site_gestion
						from   CFG..ZON_REG ZON, CFG..RES_POR POR, CFG..EQT_GEN GEN
						where ZON.autoroute = POR.autoroute
						and ZON.debut >= POR.PR_debut
						and ZON.debut <= POR.PR_fin
					/*	and ( POR.district = @va_NumSecteur_in or @va_NumSecteur_in = XDC_CI)*/
						and GEN.type = XDC_EQT_ZONE_REGUL
						and GEN.numero = ZON.numero
                      
					end   

                       

#ifdef CI      
            else

             /*A
            **  Travail dans la base CFT
            */   
      
				if @va_Basedd_in = XDC_BASE_CFT
            
					begin
                        /*A
                        ** Lecture des donnees dans ZON_REG
                        */

						select distinct
						ZON.numero, 
						ZON.nom, 
						ZON.autoroute, 
						ZON.sens,
						debut, 
						fin,
						vitesse_amont, 
						inhibition,
						mode_horaire,
						GEN.site_gestion
						from   CFT..ZON_REG ZON, CFT..RES_POR POR, CFT..EQT_GEN GEN
						where ZON.autoroute = POR.autoroute
						and ZON.debut >= POR.PR_debut
						and ZON.debut <= POR.PR_fin
					/*	and ( POR.district = @va_NumSecteur_in or @va_NumSecteur_in = XDC_CI)*/
						and GEN.type = XDC_EQT_ZONE_REGUL
						and GEN.numero = ZON.numero
					end   
  

#endif
      
	    end
	    else
	    begin
	                /*A
            **  Travail dans la base CFG
            */   
      
            /*A
            ** Lecture des donnees dans ZON_REG
            */

			select distinct
			ZON.numero, 
			ZON.nom, 
			ZON.autoroute, 
			ZON.sens,
			debut, 
			fin,
			vitesse_amont, 
			inhibition,
			mode_horaire,
			GEN.site_gestion
			from   CFG..ZON_REG ZON, CFG..RES_POR POR, CFG..EQT_GEN GEN
			where ZON.autoroute = POR.autoroute
			and ZON.debut >= POR.PR_debut
			and ZON.debut <= POR.PR_fin
			/*and ( POR.district = @va_NumSecteur_in or @va_NumSecteur_in = XDC_CI)*/
			and GEN.type = XDC_EQT_ZONE_REGUL
			and GEN.numero = ZON.numero
	    end
    end         
     
    return XDC_OK
         
go  
