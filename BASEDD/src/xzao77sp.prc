/*E*/
/* Fichier : $Id: xzao77sp.prc,v 1.3 2018/11/06 09:49:11 devgfi Exp $        $Revision: 1.3 $        $Date: 2018/11/06 09:49:11 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao77sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation  1.1, 1.2
* JPL		16/10/18 : Filtrage des equipements marques comme supprimes (DEM 1305)  1.3
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao77sp.h" */

/* static char *version = "@(#)xzao77.c	1.1 23/18/94 : xzao77" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des Mini GTC pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;77
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO77SP
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
* Retourne la liste des Mini GTC de la Machine 
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
* Retourne les parametres dans la structure XZAT_MiniGTC
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Machine		smallint
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null 
   or @va_NomMachine_in = null
   
      return XDC_ARG_INV
   
   else   
   
   /*A
   ** Les parametres d'entree ne sont pas nuls
   */
   
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         
         /*A
         ** Recherche du numero de la machine passee en argument
         */
         
         select @vl_Machine = numero from CFG..EQT_GEN
                 where nom  = @va_NomMachine_in
                   and type = XDC_EQT_MAC
                   
         
         /*A 
         ** Recherche des Mini GTC de la base lies a plusieurs serveurs
         */
         
         select distinct GTC.numero, GEN.nom, GTC.liaison_serie, GTC.rgs, TOR.numero_de_bit, TOR.alarme, TOR.alerte, GEN3.numero, GEN3.nom from CFG..EQT_GEN GEN, CFG..EQT_GEN GEN2, CFG..EQT_GEN GEN3, CFG..EQT_GTC GTC, CFG..EQT_TOR TOR
            where GEN.type         = XDC_EQT_SRV
              and GEN.maitre       = @vl_Machine
              and GEN.type_maitre  = XDC_MAI_SRV
              and GEN2.type        = XDC_EQT_GTC
              and GEN2.maitre      = GEN.numero
              and GEN2.type_maitre = XDC_MAI_GTC
              and GEN2.eqt_supprime = XDC_FAUX
              and GTC.numero       = GEN2.numero
              and GEN3.type        = XDC_EQT_TOR
              and GEN3.maitre      = GEN2.numero
              and GEN3.type_maitre = XDC_MAI_TOR
              and TOR.numero	   = GEN3.numero
           
           order by GEN.nom, GTC.numero, GEN3.numero   
              
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         
         /*A
         ** Recherche du numero de la machine passee en argument
         */
         
         select @vl_Machine = numero from CFT..EQT_GEN
                 where nom  = @va_NomMachine_in
                   and type = XDC_EQT_MAC
                   
         
         /*A 
         ** Recherche des Mini GTC de la base lies a plusieurs serveurs
         */
         
         select distinct GTC.numero, GEN.nom, GTC.liaison_serie, GTC.rgs, TOR.numero_de_bit, TOR.alarme, TOR.alerte, GEN3.numero, GEN3.nom from CFT..EQT_GEN GEN, CFT..EQT_GEN GEN2, CFT..EQT_GEN GEN3, CFT..EQT_GTC GTC, CFT..EQT_TOR TOR
            where GEN.type         = XDC_EQT_SRV
              and GEN.maitre       = @vl_Machine
              and GEN.type_maitre  = XDC_MAI_SRV
              and GEN2.type        = XDC_EQT_GTC
              and GEN2.maitre      = GEN.numero
              and GEN2.type_maitre = XDC_MAI_GTC
              and GEN2.eqt_supprime = XDC_FAUX
              and GTC.numero       = GEN2.numero
              and GEN3.type        = XDC_EQT_TOR
              and GEN3.maitre      = GEN2.numero
              and GEN3.type_maitre = XDC_MAI_TOR
              and TOR.numero	   = GEN3.numero
           
           order by GEN.nom, GTC.numero, GEN3.numero   
              
      end       
#endif

   end
      
   return XDC_OK
         
go  
