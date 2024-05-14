/*E*/
/*Fichier : @(#)xzao201sp.prc	1.2      Release : 1.2        Date : 12/08/94
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao201sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao201sp.h" */

/* static char *version = "@(#)xzao201.c	1.1 23/18/94 : xzao201" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des sequences echangeur connues
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;201
   @va_Basedd_in        	char(3),
   @va_TypeObjet_in		tinyint,
   @va_NumObjet_in		smallint,
   @va_NomVille_in		char(25),
   @va_NumFicheAnnuaire_out	smallint output
   as 

/* 
*
* SP	XZAO201SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in
* XDY_Octet 	va_TypeObjet_in
* XDY_Mot	va_NumObjet_in
* XDY_Nom	va_NomVille_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Mot	va_NumFicheAnnuaire_out
*
* VALEUR RENDUE :
*
* Recherche l'interlocuteur correspondant a l'objet graphique
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
* volcic	07 Oct 1994	: Creation
------------------------------------------------------*/

   select @va_NumFicheAnnuaire_out = null
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV  
   */
      
   if @va_Basedd_in = null
   or @va_TypeObjet_in = null
   or ( @va_NumObjet_in = null and @va_NomVille_in = null )
    
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
         ** Recherche de l'interlocuteur correspondant a l'objet graphique
         */
         
         if @va_TypeObjet_in = XDC_RES_PEAGE
          
            select @va_NumFicheAnnuaire_out = ANN.numero from CFG..ANN_FIC ANN, CFG..RES_PEA PEA
              where ANN.numero = PEA.gare_de_peage
                and PEA.numero = @va_NumObjet_in
                 
         else
         
            if @va_TypeObjet_in = XDC_RES_AIRE
          
            select @va_NumFicheAnnuaire_out = ANN.numero from CFG..ANN_FIC ANN, CFG..RES_AIR AIR
              where ANN.numero = AIR.sous_concessionnaire
                and AIR.numero = @va_NumObjet_in
            
            else
         
               if @va_TypeObjet_in = XDC_RES_PS
          
               select @va_NumFicheAnnuaire_out = ANN.numero from CFG..ANN_FIC ANN, CFG..RES_SUP PS
                 where ANN.numero = PS.interlocuteur
                   and PS.numero  = @va_NumObjet_in 
           

               else
         
                  if @va_TypeObjet_in = XDC_RES_RT
          
                  select @va_NumFicheAnnuaire_out = ANN.numero from CFG..ANN_FIC ANN, CFG..RES_TRN RT
                    where ANN.numero = RT.interlocuteur
                      and RT.numero  = @va_NumObjet_in 
           
                  else
                         
                     if @va_TypeObjet_in = XDC_RES_VILLE
          
                     select @va_NumFicheAnnuaire_out = ANN.numero from CFG..ANN_FIC ANN, CFG..RES_VIL VIL
                       where VIL.nom    = @va_NomVille_in
                         and ANN.numero = VIL.interlocuteur 
           
                     else
                         
                        if @va_TypeObjet_in = XDC_RES_ECHANGEUR
          
                        select @va_NumFicheAnnuaire_out = ANN.numero from CFG..ANN_FIC ANN, CFG..RES_ECH ECH
                          where ANN.numero = ECH.gare_de_peage
                            and ECH.numero = @va_NumObjet_in
           
             
      end

#ifdef CI      
      
      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Recherche de l'interlocuteur correspondant a l'objet graphique
         */
         
         if @va_TypeObjet_in = XDC_RES_PEAGE
          
            select @va_NumFicheAnnuaire_out = ANN.numero from CFT..ANN_FIC ANN, CFT..RES_PEA PEA
              where ANN.numero = PEA.gare_de_peage
                and PEA.numero = @va_NumObjet_in
                 
         else
         
            if @va_TypeObjet_in = XDC_RES_AIRE
          
            select @va_NumFicheAnnuaire_out = ANN.numero from CFT..ANN_FIC ANN, CFT..RES_AIR AIR
              where ANN.numero = AIR.sous_concessionnaire
                and AIR.numero = @va_NumObjet_in
            
            else
         
               if @va_TypeObjet_in = XDC_RES_PS
          
               select @va_NumFicheAnnuaire_out = ANN.numero from CFT..ANN_FIC ANN, CFT..RES_SUP PS
                 where ANN.numero = PS.interlocuteur
                   and PS.numero  = @va_NumObjet_in 
           

               else
         
                  if @va_TypeObjet_in = XDC_RES_RT
          
                  select @va_NumFicheAnnuaire_out = ANN.numero from CFT..ANN_FIC ANN, CFT..RES_TRN RT
                    where ANN.numero = RT.interlocuteur
                      and RT.numero  = @va_NumObjet_in 
           
                  else
                         
                     if @va_TypeObjet_in = XDC_RES_VILLE
          
                     select @va_NumFicheAnnuaire_out = ANN.numero from CFT..ANN_FIC ANN, CFT..RES_VIL VIL
                       where VIL.nom    = @va_NomVille_in
                         and ANN.numero = VIL.interlocuteur 
           
                     else
                         
                        if @va_TypeObjet_in = XDC_RES_ECHANGEUR
          
                        select @va_NumFicheAnnuaire_out = ANN.numero from CFT..ANN_FIC ANN, CFT..RES_ECH ECH
                          where ANN.numero = ECH.gare_de_peage
                            and ECH.numero = @va_NumObjet_in
           
             
      end
      
#endif
      
   end
      
   return XDC_OK
         
go  
