/*E*/
/*Fichier : $Id: xzao63sp.prc,v 1.6 2018/07/11 09:32:31 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2018/07/11 09:32:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao63sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao63sp.h" */

/* static char *version = "@(#)xzao63.c	1.1 23/18/94 : xzao63" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un Influence_Station dans la table RES_INF
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;63
   @va_Basedd_in        	char(3),
   @va_Autoroute_in		char(4),
   @va_NomStation_in		char(10),
   @va_Sens_in			tinyint,
   @va_PRdebut_in		int,
   @va_PRfin_in			int,	
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO63SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_Nom	va_NomPortion_in
* XDY_NomEqt	va_NomStation_in
* XDY_Sens	va_Sens_in
* XDY_PR	va_PRdebut_in
* XDY_PR	va_PRfin_in
* XDY_Booleen	va_Creation_in			
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un pente a une base
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
* Insertion dans la table RES_INF (ZONE D INFLUENCE STATION)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	12 Sep 1994	: Creation
* volcic	09 Nov 1994	: Ajout #ifdef CI (v1.2)
* volcic	23 Oct 1995	: Modif decpoupage des zones d'influence en fonction des portions (v1.4)
* LCL		05/07/17	: modif PRA
* LCL      	11/07/18	: correction syntaxe from
* GGY		26/09/23	:	Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_NumStation	smallint,
   @vl_NumPortion	smallint,
   @vl_Numero		smallint,
   @vl_PRdebut		int,
   @vl_PRfin		int,
   @vl_PRdeb		int,
   @vl_PRf			int,
   @vl_Cpt			int,
   @vl_Nb			int,
   @vl_NumSt		smallint,
   @vl_Nb_Portions	smallint,
   @vl_Site			char(10)

   select @va_Resultat_out = XDC_ECHEC
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null 
   or @va_Autoroute_in = null 
   or @va_NomStation_in = null 
   or @va_Sens_in = null 
   or @va_PRdebut_in = null 
   or @va_PRfin_in = null 
   
      return XDC_ARG_INV
   
   else   
   begin

		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin
      
      select @vl_Cpt = 0
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin 
         
         select @vl_Autoroute = numero from CFG..RES_AUT
           where nom = @va_Autoroute_in
           
         select @vl_Numero = numero from CFG..EQT_GEN
           where nom = @va_NomStation_in
             and type = XDC_EQT_RAD
             and autoroute = @vl_Autoroute
           
         select @vl_NumStation = station from CFG..RES_CHA
           where station = @vl_Numero
             and sens    = @va_Sens_in
         
         select numero, PR_debut, PR_fin into #TMP_POR from CFG..RES_POR
           where autoroute = @vl_Autoroute
             and PR_debut <= @va_PRfin_in
             and PR_fin   >= @va_PRdebut_in
           order by PR_debut    
         
         select @vl_Nb = @@rowcount
         
         if @vl_Nb != 0 and @vl_NumStation != null
         begin
            
            select @vl_NumSt = portion from CFG..RES_INF
              where station  = @vl_NumStation
                and sens     = @va_Sens_in
                
            /*A
            ** La zone d'influence n'existe pas
            */
            
            if @vl_NumSt = null
            begin
            
               /*A
               ** Si l'indicateur Creation est positionne
               */
               
               if @va_Creation_in = XDC_VRAI
               begin
               
                  /*A
                  **  Insertion de la zone d'influence argument dans la table CFG..RES_INF de la base
                  **  passee en argument, si celui ci n'existe pas et Creation positionne
                  */
                  
                  declare Portion cursor
                  for select numero , PR_debut, PR_fin from #TMP_POR
                  
                  open Portion
                  
                  fetch Portion into @vl_NumPortion, @vl_PRdebut, @vl_PRfin
                  
                  while (@@sqlstatus = 0)
                  begin
                  
                     select @vl_Cpt = @vl_Cpt + 1
                     
                     if @vl_Cpt = 1
                     begin
                        if @vl_PRfin >= @va_PRfin_in
                           insert into CFG..RES_INF
                           values ( @vl_NumPortion, @vl_NumStation, @va_Sens_in, @va_PRdebut_in, @va_PRfin_in )
                        else
                           insert into CFG..RES_INF
                           values ( @vl_NumPortion, @vl_NumStation, @va_Sens_in, @va_PRdebut_in, @vl_PRfin )
                     end
                     else
                     begin         
                        if @vl_PRfin >= @va_PRfin_in
                           insert into CFG..RES_INF
                           values ( @vl_NumPortion, @vl_NumStation, @va_Sens_in, @vl_PRdebut, @va_PRfin_in )
                        else
                           insert into CFG..RES_INF
                           values ( @vl_NumPortion, @vl_NumStation, @va_Sens_in, @vl_PRdebut, @vl_PRfin )
                     end   
                           
                     select @va_Resultat_out = XDC_AJOUT
                     
                     fetch Portion into @vl_NumPortion, @vl_PRdebut, @vl_PRfin
                  end 
                  close Portion  
               end
            end
            else
            /*A
            ** La zone d'influence existe deja
            */
            begin
               
               /*A
               ** Si l'indicateur Creation est positionne
               */
               
               if @va_Creation_in = XDC_VRAI
               begin
               
                  /*A
                  ** Mise ajour du nombre de portions correspondant aux PR arguments
                  */
                  select @vl_Nb_Portions = count(*) from #TMP_POR
                  
                  /*A
                  ** Suppression des parties de zone d'influence situees sur les portions ne correspondant plus aux PR arguments
                  */
                  delete  CFG..RES_INF
                  where portion not in (select numero from #TMP_POR)
                    and station = @vl_NumStation 
                    and sens    = @va_Sens_in
                  
                  declare Portion cursor
                  for select numero , PR_debut, PR_fin from #TMP_POR
                  
                  open Portion
                  
                  fetch Portion into @vl_NumPortion, @vl_PRdebut, @vl_PRfin
                  
                  while (@@sqlstatus = 0)
                  begin
                     select @vl_Cpt = @vl_Cpt + 1
                     
                     select @vl_PRdeb = NULL
                     
                     select @vl_PRdeb = PR_debut from CFG..RES_INF
                        where portion = @vl_NumPortion
                          and station = @vl_NumStation 
                          and sens    = @va_Sens_in
                     
                     if @vl_PRdeb = NULL 
                     begin
                        if @vl_Cpt = 1
                           insert into CFG..RES_INF
                           values ( @vl_NumPortion, @vl_NumStation, @va_Sens_in, @va_PRdebut_in, @vl_PRfin )
                        else
                           insert into CFG..RES_INF
                           values ( @vl_NumPortion, @vl_NumStation, @va_Sens_in, @vl_PRdebut, @vl_PRfin )
                     end
                     else
                     begin
                        if @vl_Cpt = 1
                           update CFG..RES_INF
                           set PR_debut  = @va_PRdebut_in
                           where portion = @vl_NumPortion
                             and station = @vl_NumStation 
                             and sens    = @va_Sens_in
                         else
                           update CFG..RES_INF
                           set PR_debut  = @vl_PRdebut
                           where portion = @vl_NumPortion
                             and station = @vl_NumStation 
                             and sens    = @va_Sens_in
                     end   
                     
                     select @vl_PRf = NULL
                        
                     select @vl_PRf = PR_fin from CFG..RES_INF
                        where portion = @vl_NumPortion
                          and station = @vl_NumStation 
                          and sens    = @va_Sens_in
                        
                     if @vl_PRfin = NULL 
                     begin
                        if @vl_Cpt = @vl_Nb_Portions
                           insert into CFG..RES_INF
                           values ( @vl_NumPortion, @vl_NumStation, @va_Sens_in, @vl_PRdebut, @va_PRfin_in )
                        else
                           insert into CFG..RES_INF
                           values ( @vl_NumPortion, @vl_NumStation, @va_Sens_in, @vl_PRdebut, @vl_PRfin )
                     end
                     else
                     begin
                        if @vl_Cpt = @vl_Nb_Portions
                           update CFG..RES_INF
                           set PR_fin  = @va_PRfin_in
                           where portion = @vl_NumPortion
                             and station = @vl_NumStation 
                             and sens    = @va_Sens_in
                        else
                           update CFG..RES_INF
                           set PR_fin  = @vl_PRfin
                           where portion = @vl_NumPortion
                             and station = @vl_NumStation 
                             and sens    = @va_Sens_in
                     end                              
                                       
                     fetch Portion into @vl_NumPortion, @vl_PRdebut, @vl_PRfin
                  
                  end 
                  
                  close Portion
                  
                  select @va_Resultat_out = XDC_MODIF 
               
               end
               
               else
               
               /*A
               ** Si l'indicateur Creation n'est pas positionne
               */
               
               begin
               
                  /*A
                  **  Suppression de la zone d'influence argument dans la table CFG..RES_INF de la base
                  **  passee en argument, si celui ci n'existe pas et Creation positionne
                  */
                  delete CFG..RES_INF
                  where station = @vl_NumStation

                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                  */
         
                  select @va_Resultat_out = XDC_SUPPR
               end 
            end
         end
         
         else
           
            /*A 
            ** La portion ou la station passee en argument n'existe pas
            */
            
            /*A
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
         
         select @vl_Autoroute = numero from CFT..RES_AUT
           where nom = @va_Autoroute_in
           
         select @vl_Numero = numero from CFT..EQT_GEN
           where nom = @va_NomStation_in
             and type = XDC_EQT_RAD
             and autoroute = @vl_Autoroute
           
         select @vl_NumStation = station from CFT..RES_CHA
           where station = @vl_Numero
             and sens    = @va_Sens_in
         
         select numero, PR_debut, PR_fin into #TMP_POR1 from CFT..RES_POR
           where autoroute = @vl_Autoroute
             and PR_debut <= @va_PRfin_in
             and PR_fin   >= @va_PRdebut_in
           order by PR_debut    
         
         select @vl_Nb = @@rowcount
         
         if @vl_Nb != 0 and @vl_NumStation != null
         begin
            
            select @vl_NumSt = portion from CFT..RES_INF
              where station  = @vl_NumStation
                and sens     = @va_Sens_in
                
            /*A
            ** La zone d'influence n'existe pas
            */
            
            if @vl_NumSt = null
            begin
            
               /*A
               ** Si l'indicateur Creation est positionne
               */
               
               if @va_Creation_in = XDC_VRAI
               begin
               
                  /*A
                  **  Insertion de la zone d'influence argument dans la table CFT..RES_INF de la base
                  **  passee en argument, si celui ci n'existe pas et Creation positionne
                  */
                  
                  declare Portion cursor
                  for select numero , PR_debut, PR_fin from #TMP_POR1
                  
                  open Portion
                  
                  fetch Portion into @vl_NumPortion, @vl_PRdebut, @vl_PRfin
                  
                  while (@@sqlstatus = 0)
                  begin
                  
                     select @vl_Cpt = @vl_Cpt + 1
                     
                     if @vl_Cpt = 1
                     begin
                        if @vl_PRfin >= @va_PRfin_in
                           insert into CFT..RES_INF
                           values ( @vl_NumPortion, @vl_NumStation, @va_Sens_in, @va_PRdebut_in, @va_PRfin_in )
                        else
                           insert into CFT..RES_INF
                           values ( @vl_NumPortion, @vl_NumStation, @va_Sens_in, @va_PRdebut_in, @vl_PRfin )
                     end
                     else
                     begin         
                        if @vl_PRfin >= @va_PRfin_in
                           insert into CFT..RES_INF
                           values ( @vl_NumPortion, @vl_NumStation, @va_Sens_in, @vl_PRdebut, @va_PRfin_in )
                        else
                           insert into CFT..RES_INF
                           values ( @vl_NumPortion, @vl_NumStation, @va_Sens_in, @vl_PRdebut, @vl_PRfin )
                     end   
                           
                     select @va_Resultat_out = XDC_AJOUT
                     
                     fetch Portion into @vl_NumPortion, @vl_PRdebut, @vl_PRfin
                  end 
                  close Portion  
               end
            end
            else
            /*A
            ** La zone d'influence existe deja
            */
            begin
               
               /*A
               ** Si l'indicateur Creation est positionne
               */
               
               if @va_Creation_in = XDC_VRAI
               begin
               
                  /*A
                  ** Mise ajour du nombre de portions correspondant aux PR arguments
                  */
                  select @vl_Nb_Portions = count(*) from #TMP_POR1
                  
                  /*A
                  ** Suppression des parties de zone d'influence situees sur les portions ne correspondant plus aux PR arguments
                  */
                  delete  CFT..RES_INF
                  where portion not in (select numero from #TMP_POR1)
                    and station = @vl_NumStation 
                    and sens    = @va_Sens_in
                  
                  declare Portion cursor
                  for select numero , PR_debut, PR_fin from #TMP_POR1
                  
                  open Portion
                  
                  fetch Portion into @vl_NumPortion, @vl_PRdebut, @vl_PRfin
                  
                  while (@@sqlstatus = 0)
                  begin
                     select @vl_Cpt = @vl_Cpt + 1
                     
                     select @vl_PRdeb = NULL
                     
                     select @vl_PRdeb = PR_debut from CFT..RES_INF
                        where portion = @vl_NumPortion
                          and station = @vl_NumStation 
                          and sens    = @va_Sens_in
                     
                     if @vl_PRdeb = NULL 
                     begin
                        if @vl_Cpt = 1
                           insert into CFT..RES_INF
                           values ( @vl_NumPortion, @vl_NumStation, @va_Sens_in, @va_PRdebut_in, @vl_PRfin )
                        else
                           insert into CFT..RES_INF
                           values ( @vl_NumPortion, @vl_NumStation, @va_Sens_in, @vl_PRdebut, @vl_PRfin )
                     end
                     else
                     begin
                        if @vl_Cpt = 1
                           update CFT..RES_INF
                           set PR_debut  = @va_PRdebut_in
                           where portion = @vl_NumPortion
                             and station = @vl_NumStation 
                             and sens    = @va_Sens_in
                         else
                           update CFT..RES_INF
                           set PR_debut  = @vl_PRdebut
                           where portion = @vl_NumPortion
                             and station = @vl_NumStation 
                             and sens    = @va_Sens_in
                     end   
                     
                     select @vl_PRf = NULL
                        
                     select @vl_PRf = PR_fin from CFT..RES_INF
                        where portion = @vl_NumPortion
                          and station = @vl_NumStation 
                          and sens    = @va_Sens_in
                        
                     if @vl_PRfin = NULL 
                     begin
                        if @vl_Cpt = @vl_Nb_Portions
                           insert into CFT..RES_INF
                           values ( @vl_NumPortion, @vl_NumStation, @va_Sens_in, @vl_PRdebut, @va_PRfin_in )
                        else
                           insert into CFT..RES_INF
                           values ( @vl_NumPortion, @vl_NumStation, @va_Sens_in, @vl_PRdebut, @vl_PRfin )
                     end
                     else
                     begin
                        if @vl_Cpt = @vl_Nb_Portions
                           update CFT..RES_INF
                           set PR_fin  = @va_PRfin_in
                           where portion = @vl_NumPortion
                             and station = @vl_NumStation 
                             and sens    = @va_Sens_in
                        else
                           update CFT..RES_INF
                           set PR_fin  = @vl_PRfin
                           where portion = @vl_NumPortion
                             and station = @vl_NumStation 
                             and sens    = @va_Sens_in
                     end                              
                                       
                     fetch Portion into @vl_NumPortion, @vl_PRdebut, @vl_PRfin
                  
                  end 
                  
                  close Portion
                  
                  select @va_Resultat_out = XDC_MODIF 
               
               end
               
               else
               
               /*A
               ** Si l'indicateur Creation n'est pas positionne
               */
               
               begin
               
                  /*A
                  **  Suppression de la zone d'influence argument dans la table CFT..RES_INF de la base
                  **  passee en argument, si celui ci n'existe pas et Creation positionne
                  */
                  delete CFT..RES_INF
                  where station = @vl_NumStation

                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                  */
         
                  select @va_Resultat_out = XDC_SUPPR
               end 
            end
         end
         
         else
           
            /*A 
            ** La portion ou la station passee en argument n'existe pas
            */
            
            /*A
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
            select @va_Resultat_out = XDC_ECHEC
      end
      
#endif
            
      end
      
      else
      
      /*A
      ** Le serveur n'est pas celui du CI
      ** return (XDC_NOK)
      */
         return (XDC_NOK)
            
   end
      
   return XDC_OK
         
go  
