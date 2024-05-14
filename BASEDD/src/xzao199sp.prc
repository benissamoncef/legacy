/*E*/
/*Fichier : $Id: xzao199sp.prc,v 1.3 2018/07/11 10:44:58 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2018/07/11 10:44:58 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao199sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* configuration du reseau
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao199sp.h" */

/* static char *version = "@(#)xzao199.c	1.1 22/01/96 : xzao199" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  supprimer le lien entre un interlocuteur et un objet graphique dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
* SP	XZAO199_Supprimer_Interlocuteur
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Octet		va_TypeObjet_in
* XDY_Mot		va_NumeroObjet_in
* XDY_Nom		va_NomVille_in
* XDY_Mot		va_NumeroFicheAnnuaire_in
* 
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* supprimer l'interlocuteur pour un objet graphique en base
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
* supprimer le lien entre un interlocuteur et un objet graphique
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* torregrossa	22 Jan 1996	: Creation
* LCL           11/07/18	: correction syntaxe from
* GGY			26/09/23	: Ajout district CA (DEM483)
* GGY			17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
use PRC
go

create procedure XZAO;199
   @va_Basedd_in        	char(3),
   @va_TypeObjet_in		tinyint,
   @va_NumeroObjet_in		smallint,
   @va_NomVille_in		char(25),
   @va_NumeroFicheAnnuaire_in	smallint,
   @va_Resultat_out		int output
   as 

declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Interlocuteur	smallint,
   @vl_Site				char(10)
   
   select @va_Resultat_out = XDC_ECHEC
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_TypeObjet_in = null 
   or ( @va_NumeroObjet_in = null and @va_NomVille_in = null )
   or @va_NumeroFicheAnnuaire_in = null
   
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
            ** Recherche de l'interlocuteur correspondant a l'objet graphique
            */
         
            if @va_TypeObjet_in = XDC_RES_PEAGE
          
               update CFG..RES_PEA
               set gare_de_peage = null
               where numero = @va_NumeroObjet_in and
			gare_de_peage = @va_NumeroFicheAnnuaire_in
              
            else
         
            if @va_TypeObjet_in = XDC_RES_AIRE
               
               update CFG..RES_AIR
               set sous_concessionnaire = null
               where numero = @va_NumeroObjet_in and
				sous_concessionnaire = @va_NumeroFicheAnnuaire_in
              
            else
         
               if @va_TypeObjet_in = XDC_RES_PS
          
                  update CFG..RES_SUP
                  set interlocuteur = null
                  where numero = @va_NumeroObjet_in  and
				interlocuteur = @va_NumeroFicheAnnuaire_in
           

               else
         
                  if @va_TypeObjet_in = XDC_RES_RT
                     
                     update CFG..RES_TRN
                     set interlocuteur = null
                     where numero = @va_NumeroObjet_in  and
				interlocuteur = @va_NumeroFicheAnnuaire_in
           
                  else
                         
                     if @va_TypeObjet_in = XDC_RES_VILLE
                        update CFG..RES_VIL
                        set interlocuteur = null
                        where nom = @va_NomVille_in  and
				interlocuteur = @va_NumeroFicheAnnuaire_in
           
      
          
              if @@rowcount = 1
      
              /*A
              ** La modification a eu lieu
              ** Affectation du parametre de sortie Resultat a XDC_MODIF
              */
            
                 select @va_Resultat_out = XDC_MODIF
      
              else            
                     
               /*A
               ** La modification n'a pas eu lieu
               ** Affectation du parametre de sortie Resultat a XDC_ECHEC
               */
            
                select @va_Resultat_out = XDC_ECHEC
               
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
          
               update CFT..RES_PEA
               set gare_de_peage = null
               where numero = @va_NumeroObjet_in and
			gare_de_peage = @va_NumeroFicheAnnuaire_in
              
            else
         
            if @va_TypeObjet_in = XDC_RES_AIRE
               
               update CFT..RES_AIR
               set sous_concessionnaire = null
               where numero = @va_NumeroObjet_in and
			sous_concessionnaire = @va_NumeroFicheAnnuaire_in
              
            else
         
               if @va_TypeObjet_in = XDC_RES_PS
          
                  update CFT..RES_SUP
                  set interlocuteur = null
                  where numero = @va_NumeroObjet_in  and
				interlocuteur = @va_NumeroFicheAnnuaire_in
           

               else
         
                  if @va_TypeObjet_in = XDC_RES_RT
                     
                     update CFT..RES_TRN
                     set interlocuteur = null
                     where numero = @va_NumeroObjet_in  and
				interlocuteur = @va_NumeroFicheAnnuaire_in
           
                  else
                         
                     if @va_TypeObjet_in = XDC_RES_VILLE
                        update CFT..RES_VIL
                        set interlocuteur = null
                        where nom = @va_NomVille_in  and
				interlocuteur = @va_NumeroFicheAnnuaire_in
           
      
          
              if @@rowcount = 1
      
              /*A
              ** La modification a eu lieu
              ** Affectation du parametre de sortie Resultat a XDC_MODIF
              */
            
                 select @va_Resultat_out = XDC_MODIF
      
              else            
                     
               /*A
               ** La modification n'a pas eu lieu
               ** Affectation du parametre de sortie Resultat a XDC_ECHEC
               */
            
                select @va_Resultat_out = XDC_ECHEC
               
         end
#endif         
      
      end
   end         
     
   return XDC_OK
         
go  
