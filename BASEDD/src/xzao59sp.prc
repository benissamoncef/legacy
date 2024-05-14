/*E*/
/*Fichier : $Id: xzao59sp.prc,v 1.9 2018/07/11 09:29:31 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2018/07/11 09:29:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao59sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao59sp.h" */

/* static char *version = "@(#)xzao59.c	1.1 23/18/94 : xzao59" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un Segment dans la table SEGMENT HOMOGENE
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;59
   @va_Basedd_in        	char(3),
   @va_NomPortion_in		char(5),
   @va_PRdebut_in		int,
   @va_Sens_in			tinyint,
   @va_NombreVoies_in		smallint,
   @va_LargeurBAU_in		smallint,
   @va_EnTunnel_in		bit,
   @va_NomTunnel_in		char(25),
   @va_Gabarit_in		smallint,	
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO59SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_Nom	va_NomPortion_in
* XDY_PR	va_PRdebut_in
* XDY_Sens	va_Sens_in
* XDY_Octet	va_NombreVoies_in
* XDY_Octet	va_LargeurBAU_in
* XDY_Booleen	va_EnTunnel_in
* XDY_Nom	va_NomTunnel_in
* XDY_Mot	va_Gabarit_in
* XDY_Booleen	va_Creation_in			
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un segment homogene a une base
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
* Insertion dans la table RES_SEG (SEGMENT HOMOGENE)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic		12 Sep 1994	: Creation
* volcic		18 Oct 1994	: Ajout travail CFT (v1.2)
* volcic		09 Nov 1994	: Ajout #ifdef CI (v1.3)
* volcic		15 Jan 1995	: Modif va_Resultat_out (v1.4)
* volcic		15 Jan 1995	: Modif va_Resultat_out (v1.5)
* Torregrossa	06 Dec 1996	: Ajout nom des colonnes dans insert (v1.6)
* C.T.			18 Fev 1998	: Modif test sur update suite SQL server 11.0.2.3 (1.7)
* LCL			05/07/17	: modif PRA
* LCL           11/07/18	: correction syntaxe from
* GGY			26/09/23	:	Ajout district CA (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumSegment	smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)

   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null 
   or @va_NomPortion_in = null 
   or @va_PRdebut_in = null 
   or @va_Sens_in = null 
   or @va_NombreVoies_in = null 
   or @va_LargeurBAU_in = null
   or @va_EnTunnel_in = null 
   or @va_Gabarit_in = null 
   or @va_Creation_in = null 
    
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
         select @vl_NumSegment = numero from CFG..RES_POR
            where nom = @va_NomPortion_in
             
         if @vl_NumSegment <> null
         begin
            select @vl_Numero = portion from CFG..RES_SEG
              where portion  = @vl_NumSegment 
                and PR_debut = @va_PRdebut_in
                and sens     = @va_Sens_in
              
            /*A
            **  Insertion du segment homogene argument dans la table CFG..RES_SEG de la base
            **  passee en argument, si celui ci n'existe pas et Creation positionne
            */
               
            if @vl_Numero = null
            begin
                  
               if @va_Creation_in = XDC_VRAI
               begin
                  
                  insert CFG..RES_SEG (portion, PR_debut, sens, nombre_de_voies, largeur_BAU, tunnel, nom_tunnel, gabarit)
                  values ( @vl_NumSegment, @va_PRdebut_in, @va_Sens_in, @va_NombreVoies_in, @va_LargeurBAU_in, @va_EnTunnel_in, @va_NomTunnel_in, @va_Gabarit_in)
               
                  /*A
                  ** Erreur d'insertion
                  */
                     
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                     */
            
                     select @va_Resultat_out = XDC_AJOUT
                        
                     return XDC_NOK
                  end
                  
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                  */
         
                  select @va_Resultat_out = XDC_AJOUT
               end
            end
               
            else
            
            begin
              
               if @va_Creation_in = XDC_VRAI
               begin
                  
                  /*A
                  **  Modification du segment homogene argument dans la table CFG..RES_SEG de la base
                  **  passee en argument, si celui ci n'existe pas et Creation positionne
                  */
               
                  update CFG..RES_SEG
                  set nombre_de_voies = @va_NombreVoies_in,
                      largeur_BAU = @va_LargeurBAU_in,
                      tunnel = @va_EnTunnel_in,
                      nom_tunnel = @va_NomTunnel_in,
                      gabarit = @va_Gabarit_in
                  where portion  = @vl_Numero
                    and PR_debut = @va_PRdebut_in
                    and sens     = @va_Sens_in
               
                  /*A
                  ** Erreur de modification
                  */
                     
                  if @@rowcount > 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_MODIF
                     */
         
                     select @va_Resultat_out = XDC_MODIF
                        
                     return XDC_NOK
                  end
                  
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_MODIF
                  */
         
                  select @va_Resultat_out = XDC_MODIF
               end
               
               else
               
               begin
               
               /*A
               **  Suppression du segment homogene argument dans la table CFG..RES_SEG de la base
               **  passee en argument, si celui ci n'existe pas et Creation non positionne
               */
               
                  delete CFG..RES_SEG
                  where portion  = @vl_Numero
                    and PR_debut = @va_PRdebut_in
                    and sens     = @va_Sens_in
               
                  /*A
                  ** Erreur de modification
                  */
                     
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                     */
         
                     select @va_Resultat_out = XDC_SUPPR
                        
                     return XDC_NOK
                  end
                  
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                  */
         
                  select @va_Resultat_out = XDC_SUPPR
               end
            end  
         end
         
         else
            
            select @va_Resultat_out = XDC_ECHEC
         
      end

#ifdef CI
      
      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
         select @vl_NumSegment = numero from CFT..RES_POR
            where nom = @va_NomPortion_in
             
         if @vl_NumSegment <> null
         begin
            select @vl_Numero = portion from CFT..RES_SEG
              where portion  = @vl_NumSegment 
                and PR_debut = @va_PRdebut_in
                and sens     = @va_Sens_in
              
            /*A
            **  Insertion du segment homogene argument dans la table CFT..RES_SEG de la base
            **  passee en argument, si celui ci n'existe pas et Creation positionne
            */
               
            if @vl_Numero = null
            begin
                  
               if @va_Creation_in = XDC_VRAI
               begin
                  
                  insert CFT..RES_SEG (portion, PR_debut, sens, nombre_de_voies, largeur_BAU, tunnel, nom_tunnel, gabarit)
                  values ( @vl_NumSegment, @va_PRdebut_in, @va_Sens_in, @va_NombreVoies_in, @va_LargeurBAU_in, @va_EnTunnel_in, @va_NomTunnel_in, @va_Gabarit_in)
               
                  /*A
                  ** Erreur d'insertion
                  */
                     
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                     */
            
                     select @va_Resultat_out = XDC_AJOUT
                        
                     return XDC_NOK
                  end
                  
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                  */
         
                  select @va_Resultat_out = XDC_AJOUT
               end
            end
               
            else
            
            begin
              
               if @va_Creation_in = XDC_VRAI
               begin
                  
                  /*A
                  **  Modification du segment homogene argument dans la table CFT..RES_SEG de la base
                  **  passee en argument, si celui ci n'existe pas et Creation positionne
                  */
               
                  update CFT..RES_SEG
                  set nombre_de_voies = @va_NombreVoies_in,
                      largeur_BAU = @va_LargeurBAU_in,
                      tunnel = @va_EnTunnel_in,
                      nom_tunnel = @va_NomTunnel_in,
                      gabarit = @va_Gabarit_in
                  where portion  = @vl_Numero
                    and PR_debut = @va_PRdebut_in
                    and sens     = @va_Sens_in
               
                  /*A
                  ** Erreur de modification
                  */
                     
                  if @@rowcount > 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_MODIF
                     */
         
                     select @va_Resultat_out = XDC_MODIF
                        
                     return XDC_NOK
                  end
                  
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_MODIF
                  */
         
                  select @va_Resultat_out = XDC_MODIF
               end
               
               else
               
               begin
               
               /*A
               **  Suppression du segment homogene argument dans la table CFT..RES_SEG de la base
               **  passee en argument, si celui ci n'existe pas et Creation non positionne
               */
               
                  delete CFT..RES_SEG
                  where portion  = @vl_Numero
                    and PR_debut = @va_PRdebut_in
                    and sens     = @va_Sens_in
               
                  /*A
                  ** Erreur de modification
                  */
                     
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                     */
         
                     select @va_Resultat_out = XDC_SUPPR
                        
                     return XDC_NOK
                  end
                  
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                  */
         
                  select @va_Resultat_out = XDC_SUPPR
               end
            end  
         end
         
         else
            
            select @va_Resultat_out = XDC_ECHEC
         
      end
      
#endif      
      
      end      
   end
      
   return XDC_OK
         
go  
