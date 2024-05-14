/*E*/
/* Fichier : $Id: xzao560sp.prc,v 1.2 2018/01/09 11:59:43 pc2dpdy Exp $        Release : $Revision: 1.2 $        Date : $Date: 2018/01/09 11:59:43 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao560sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		26/01/12 : Creation (DEM/1016)
* LCL		05/07/17 : Modif PRA
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao560sp.h" */

/* static char *version = "@(#)xzao560.c	1.1 23/18/94 : xzao560" ; */

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

if exists (select * from sysobjects where name = 'XZAO560' and type = 'P')
	drop procedure XZAO560
go

create procedure XZAO560
   @va_Basedd_in             char(3),
   @va_NumZone_in			 char(10),
   @va_Resultat_out			 int output
   as 

/* 
*
* SP    XZAO555SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*  					va_NumZone_in
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
* Affichage des donnees
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012	: Creation (DEM/1016)
* GGY	26/09/23	: Ajout district CA (DEM483)
* GGY	17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
	  
declare
	@vl_Site	char(10)

    select @va_Resultat_out = XDC_LU
   
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
      
    if @va_Basedd_in             = null or
	   @va_NumZone_in            = null
        
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
                        ** Lecture des donnees dans EQT_GEN  REG_VIRTUELLE ZON_REG
                        */

						select 
						GEN.type, GEN.numero, GEN.maitre, GEN.type_maitre, GEN.autoroute, GEN.PR, GEN.sens, GEN.nom, GEN.site_gestion,
						VIRTUELLE.numero, VIRTUELLE.station1, VIRTUELLE.station2, VIRTUELLE.station3, VIRTUELLE.operateur1, VIRTUELLE.operateur2, VIRTUELLE.operateur3, VIRTUELLE.inhibition
						from   CFG..EQT_GEN	GEN,
							   CFG..REG_VIRTUELLE VIRTUELLE,
							   CFG..ZON_REG ZON
						where  ZON.numero = @va_NumZone_in 
						and	   GEN.autoroute = ZON.autoroute
						and	   GEN.numero = VIRTUELLE.numero
                      
					end   

                       

#ifdef CI      
            else

             /*A
            **  Travail dans la base CFT
            */   
      
				if @va_Basedd_in = XDC_BASE_CFT
            
					begin
                        /*A
                        ** Lecture des donnees dans EQT_GEN REG_VIRTUELLE ZON_REG
                        */

						select 
						GEN.type, GEN.numero, GEN.maitre, GEN.type_maitre, GEN.autoroute, GEN.PR, GEN.sens, GEN.nom, GEN.site_gestion,
						VIRTUELLE.numero, VIRTUELLE.station1, VIRTUELLE.station2, VIRTUELLE.station3, VIRTUELLE.operateur1, VIRTUELLE.operateur2, VIRTUELLE.operateur3, VIRTUELLE.inhibition
						from   CFT..EQT_GEN	GEN,
							   CFT..REG_VIRTUELLE VIRTUELLE,
							   CFT..ZON_REG ZON
						where  ZON.numero = @va_NumZone_in 
						and	   GEN.autoroute = ZON.autoroute
						and	   GEN.numero = VIRTUELLE.numero
					end   
  

#endif
      
	    end
    end         
     
    return XDC_OK
         
go  
