/*E*/
/*Fichier : $Id: xzao790sp.prc,v 1.1 2017/03/31 17:53:13 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2017/03/31 17:53:13 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao790sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* JMG   20/03/17        creation DEM1191
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao790sp.h" */

/* static char *version = "$Id: xzao790sp.prc,v 1.1 2017/03/31 17:53:13 pc2dpdy Exp $ $Revision: 1.1 $ $Date: 2017/03/31 17:53:13 $: xzao790" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des EQT SAGA 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAO790
go

create procedure XZAO790
   @va_Basedd_in        	char(3)
   as 

/* 
*
* SP	XZAO790SP
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
* Retourne la liste des PMV de la Machine 
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
* Retourne les parametres numero de eqt SAGA, nom du ST, numero de liaison, adresse rgs
* numero de type de SAGA
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
------------------------------------------------------*/
   
   /*A
   ** Declaration des variables locales
   */
   
   
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV 
   */
      
   if @va_Basedd_in = null 
      return XDC_ARG_INV
   
   else   
   
   begin

      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
            /*A
            ** Recherche de la liste des eqt SAGA associes a plusieurs serveurs
            */
           select distinct SAGA.numero,
        	   	   rtrim(GEN.nom), 
        		   SAGA.type_saga,
        		   rtrim(SAGA.instance),
        	   	   rtrim(SAGA.fels_code),
			   rtrim(SAGA.eqt_code),
			   rtrim(SAGA.code_cmd_message),
			   rtrim(SAGA.code_cmd_luminosite),
			   rtrim(SAGA.code_cmd_picto),
			   rtrim(SAGA.code_cmd_heure),
			   GEN.site_gestion,
			   rtrim(SAGA.alarme),
			   DIS.code,
			   GEN.sens
        
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_SAGA SAGA,
		   CFG..RES_DIS DIS
          
             where GEN.type         = XDC_EQT_SAGA 
               and SAGA.numero      = GEN.numero
	       and GEN.site_gestion = DIS.numero
             
	  union
           select distinct SAGA.numero,
        	   	   rtrim(GEN.nom), 
        		   SAGA.type_saga,
        		   rtrim(SAGA.instance),
        	   	   rtrim(SAGA.fels_code),
			   rtrim(SAGA.eqt_code),
			   rtrim(SAGA.code_cmd_message),
			   rtrim(SAGA.code_cmd_luminosite),
			   rtrim(SAGA.code_cmd_picto),
			   rtrim(SAGA.code_cmd_heure),
			   GEN.site_gestion,
			   rtrim(SAGA.alarme),
			   DIS.code,
			   GEN.sens
        
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_SAGA SAGA,
		   CFG..RES_DIS DIS
		where GEN.type = XDC_EQT_RAD
		and SAGA.numero=GEN.numero
		and SAGA.type_saga= XDC_SAGA_RADT
		and GEN.site_gestion = DIS.numero
             order by rtrim(GEN.nom), SAGA.numero, SAGA.type_saga  
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
            /*A
            ** Recherche de la liste des PMV associes a plusieurs serveurs
            */
         
           select distinct SAGA.numero,
        	   	   rtrim(GEN.nom), 
        		   SAGA.type_saga,
        		   rtrim(SAGA.instance),
        	   	   rtrim(SAGA.fels_code),
			   rtrim(SAGA.eqt_code),
			   rtrim(SAGA.code_cmd_message),
			   rtrim(SAGA.code_cmd_luminosite),
			   rtrim(SAGA.code_cmd_picto),
			   rtrim(SAGA.code_cmd_heure),
			   GEN.site_gestion,
			   rtrim(SAGA.alarme),
			   DIS.code,
			   GEN.sens
        
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_SAGA SAGA,
		   CFT..RES_DIS DIS
          
             where GEN.type         = XDC_EQT_SAGA 
               and SAGA.numero       = GEN.numero 
		and GEN.site_gestion = DIS.numero
	  union
           select distinct SAGA.numero,
        	   	   rtrim(GEN.nom), 
        		   SAGA.type_saga,
        		   rtrim(SAGA.instance),
        	   	   rtrim(SAGA.fels_code),
			   rtrim(SAGA.eqt_code),
			   rtrim(SAGA.code_cmd_message),
			   rtrim(SAGA.code_cmd_luminosite),
			   rtrim(SAGA.code_cmd_picto),
			   rtrim(SAGA.code_cmd_heure),
			   GEN.site_gestion,
			   rtrim(SAGA.alarme),
			   DIS.code,
			   GEN.sens
        
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_SAGA SAGA,
		   CFT..RES_DIS DIS
		where GEN.type = XDC_EQT_RAD
		and SAGA.numero=GEN.numero
		and SAGA.type_saga= XDC_SAGA_RADT
		and GEN.site_gestion = DIS.numero
             order by rtrim(GEN.nom), SAGA.numero, SAGA.type_saga
                   
         end

#endif      
   end
      
   return XDC_OK
         
go  
