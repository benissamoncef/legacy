/*E*/
/* Fichier : $Id: xzao76sp.prc,v 1.10 2019/01/07 11:53:40 pc2dpdy Exp $        $Revision: 1.10 $        $Date: 2019/01/07 11:53:40 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao76sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
* volcic	30 Nov 1994	: Ajout #ifdef CI (v1.2)
* volcic	01 Mai 1995	: Modif recherche station amont et avale (v1.3)
* volcic	14 Sep 1995	: Modif recherche station amont et avale chaussee (v1.4)
* volcic	18 Sep 1995	: Modif recherche station amont et avale chaussee 1 et 2 (v1.5)
* B.G.	24/10/95	: gestion du sens inverse A57 (1.6)
* mismer	05 dec 1996	: Modif recherche station amont et avale avec RADT virtuelle (DEM/1252) (v1.7)
* JMG   13/11/07        : ajout site de gestion DEM715  1.8
* JPL	16/10/18 : Filtrage des equipements marques comme supprimes (DEM 1305)  1.9
* JMG	12/11/18: ajout IP DEM1306 1.10
------------------------------------------------------*/


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des RADT pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAO76
go

create procedure XZAO76
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO76SP
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
* Retourne la liste des RADT de la Machine 
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
* Retourne les parametres numero de type, nombre de caracteres, nom fichier
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_NomServeur	char(10),
   @vl_Serveur		smallint,
   @vl_Machine		smallint,
   @vl_NumSt		smallint,
   @vl_Liaison		tinyint,
   @vl_Rgs 		char(3),
   @vl_Seuils		char(10),
   @vl_Alertes		char(10),
   @vl_Donnees		bit,
   @vl_Autoroute	tinyint,
   @vl_PR		int,
   @vl_PRmin		int,
   @vl_PRmax		int,
   @vl_Sens		tinyint,
   @vl_NumStAm1		smallint,
   @vl_NumStAv1		smallint,
   @vl_NumStAm2		smallint,
   @vl_NumStAv2		smallint,
   @vl_StatusSt		int,
   @vl_StatusCh		int,
   @vl_StatusSrv	int,
   @vl_SensCha		tinyint,
   @vl_Boucle		tinyint,
   @vl_BoucleInv	tinyint,
   @vl_Voie		tinyint,
   @vl_SiteGestion	smallint,
   @vl_Poids		bit,
   @vl_type tinyint, @vl_adresseIP char(20), @vl_port int, @vl_idLCR char(20)
   
   
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null 
      return XDC_ARG_INV
   
   else   
   
   /*A
   ** Les parametres d'entree ne sont pas nuls
   */
   
   begin
      
      /*A
      ** Creation d'une table temporaire TMP_RAD
      */
         
      create table #TMP_RAD
      ( numero		smallint not null,
        serveur		char(10) not null,
        liaison		tinyint  not null,
        rgs 		char(3)  not null,
        seuils		char(10) not null,
        alertes		char(10) not null,
        donnees		bit,
        st_am1		smallint null,
        st_av1		smallint null,
        st_am2		smallint null,
        st_av2		smallint null,
        senscha		tinyint null,
        boucle		tinyint null,
        boucleinv	tinyint null,
        voie		tinyint null,
        poids		bit,
	site_gestion	tinyint null,
	type		tinyint null,
	adresse_ip	char(20) null,
	port		int null,
	id_lcr		char(20) null)
         
         
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         
         /*A
         ** Recherche du numero de la machine passee en argument
         */
         
         select @vl_Machine = numero from CFG..EQT_GEN
           where type = XDC_EQT_MAC
             and nom  = @va_NomMachine_in
             
        
         /*A
         ** Declaration d'un curseur sur la table EQT_GEN pour la recherche des
         ** serveurs de terminaux
         */
         
         declare Serveur cursor
         for select numero, nom from CFG..EQT_GEN
               where type        = XDC_EQT_SRV
                 and maitre      = @vl_Machine
                 and type_maitre = XDC_MAI_SRV
                   
         open Serveur
         
         /*A
         ** Remplissage du curseur de la table EQT_GEN
         */
         
         fetch Serveur into @vl_Serveur, @vl_NomServeur
         
         select @vl_StatusSrv = @@sqlstatus
         
         /*A
         ** Declaration d'un curseur sur la table EQT_GEN pour la recherche des
         ** stations RADT liees aux differents serveurs de terminaux
         */
         
         declare Station cursor
            for select 	GEN.numero, 
            		RAD.rgs, 
            		RAD.liaison_serie, 
            		RAD.donnees_individuelles, 
            		RAD.seuils, 
            		RAD.alertes ,
			GEN.site_gestion,
			RAD.type,
			RAD.adresse_ip,
			RAD.port,
			RAD.id_lcr
            
                from CFG..EQT_GEN GEN, CFG..EQT_RAD RAD
                  
                  where GEN.type          = XDC_EQT_RAD
                    and GEN.maitre        = @vl_Serveur
                    and GEN.type_maitre   = XDC_MAI_RAD
                    and GEN.eqt_supprime  = XDC_FAUX
                    and RAD.numero        = GEN.numero
                     
         open Station
         
         /*A
         ** Boucle tant que le curseur de la table EQT_GEN n'est pas vide
         */
         
         while (@vl_StatusSrv = 0)
         begin
         
            /*A
            ** Remplissage du curseur de la table EQT_RAD
            */
         
            fetch Station into @vl_NumSt, @vl_Rgs, @vl_Liaison, @vl_Donnees, @vl_Seuils, @vl_Alertes ,
				@vl_SiteGestion, @vl_type, @vl_adresseIP, @vl_port, @vl_idLCR
         
            select @vl_StatusSt = @@sqlstatus
         
            /*A
            ** Declaration d'un curseur sur la table RES_CHA pour la recherche des
            ** chaussee de la station RADT trouvee
            */
            
            declare Chaussee cursor
               for select sens 
            
                  from CFG..RES_CHA
                  
                    where station = @vl_NumSt
         
            /*A
            ** Boucle tant que le curseur de la table EQT_RAD n'est pas vide
            */
         
            while (@vl_StatusSt = 0)
            begin
         
               open Chaussee
         
               /*A
               ** Remplissage du curseur de la table RES_CHA
               */
         
               fetch Chaussee into @vl_Sens 
         
               select @vl_StatusCh = @@sqlstatus
         
               /*A
               ** Boucle tant que le curseur de la table RES_CHA n'est pas vide
               */
         
               select @vl_NumStAm1 = null, @vl_NumStAv1 = null, @vl_NumStAm2 = null, @vl_NumStAv2 = null
               
               while (@vl_StatusCh = 0)
               begin
         
                  /*A
                  ** Recherche du numero de station
                  */
               
                  select @vl_Autoroute = autoroute, @vl_PR = PR 
               
                  from CFG..EQT_GEN 
                 
                    where type        = XDC_EQT_RAD
                      and numero      = @vl_NumSt
                      and maitre      = @vl_Serveur
                      and type_maitre = XDC_MAI_RAD
                                  
                  if @@rowcount != 0
                  begin
               
                     if exists ( select * from CFG..RES_CHA
                                 where station = @vl_NumSt
                                   and sens = @vl_Sens )
                     begin
               
                        if ( ( @vl_Sens = XDC_SENS_SUD ) or ( @vl_Sens = XDC_SENS_NORD ) )
                        begin
                           
                           /*A
                           ** Recherche du numero de la station amont sens SUD ou sens NORD
                           */
               
                           select CHA.PR into #TMP_PR from CFG..EQT_GEN GEN, CFG..RES_CHA CHA, CFG..EQT_RAD RAD
                             where GEN.type       = XDC_EQT_RAD
                               and GEN.eqt_supprime = XDC_FAUX
                               and GEN.autoroute  = @vl_Autoroute
                               and RAD.numero	  = GEN.numero
                               and RAD.liaison_serie > 0	  
                               and CHA.station    = GEN.numero
                               and CHA.sens       = @vl_Sens
                               and CHA.PR 	  < @vl_PR 
                    
                           select @vl_PRmax = max(PR) from #TMP_PR
                      
                           if ( @vl_Sens = XDC_SENS_SUD )
                        
                              select @vl_NumStAm1 = CHA.station from CFG..EQT_GEN GEN, CFG..RES_CHA CHA
                                where GEN.type       = XDC_EQT_RAD
                                  and GEN.eqt_supprime = XDC_FAUX
                                  and GEN.autoroute  = @vl_Autoroute
                                  and CHA.station    = GEN.numero
                                  and CHA.sens       = @vl_Sens	   
                                  and CHA.PR	     = @vl_PRmax
                           else
                              if ( @vl_Sens = XDC_SENS_NORD )
                        
                                 select @vl_NumStAm2 = CHA.station from CFG..EQT_GEN GEN, CFG..RES_CHA CHA
                                   where GEN.type       = XDC_EQT_RAD
                                     and GEN.eqt_supprime = XDC_FAUX
                                     and GEN.autoroute  = @vl_Autoroute
                                     and CHA.station    = GEN.numero
                                     and CHA.station    = GEN.numero
                                     and CHA.sens       = @vl_Sens	   
                                     and CHA.PR	        = @vl_PRmax
                        
                           delete #TMP_PR
                  
                           /*A
                           ** Recherche du numero de la station aval sens SUD ou sens NORD
                           */
                  
                           insert into #TMP_PR select CHA.PR from CFG..EQT_GEN GEN, CFG..RES_CHA CHA, CFG..EQT_RAD RAD
                             where GEN.type       = XDC_EQT_RAD
                               and GEN.eqt_supprime = XDC_FAUX
                               and GEN.autoroute  = @vl_Autoroute
                               and RAD.numero	  = GEN.numero
                               and RAD.liaison_serie > 0	  
                               and CHA.station    = GEN.numero
                               and CHA.sens       = @vl_Sens
                               and CHA.PR 	  > @vl_PR 
                  
                           select @vl_PRmin = min(PR) from #TMP_PR
                    
                           if ( @vl_Sens = XDC_SENS_SUD )
                        
                              select @vl_NumStAv1 = CHA.station from CFG..EQT_GEN GEN, CFG..RES_CHA CHA
                                where GEN.type       = XDC_EQT_RAD
                                  and GEN.eqt_supprime = XDC_FAUX
                                  and GEN.autoroute  = @vl_Autoroute
                                  and CHA.station    = GEN.numero
                                  and CHA.sens       = @vl_Sens	   
                                  and CHA.PR	     = @vl_PRmin
                           else
                              if ( @vl_Sens = XDC_SENS_NORD )
                        
                                 select @vl_NumStAv2 = CHA.station from CFG..EQT_GEN GEN, CFG..RES_CHA CHA
                                   where GEN.type       = XDC_EQT_RAD
                                     and GEN.eqt_supprime = XDC_FAUX
                                     and GEN.autoroute  = @vl_Autoroute
                                     and CHA.station    = GEN.numero
                                     and CHA.sens       = @vl_Sens	   
                                     and CHA.PR	        = @vl_PRmin
        
                           drop table #TMP_PR
                  
                        end
                     end
                  end
                  /*A
                  ** Remplissage du curseur de la table EQT_RAD
                  */
         
                  fetch Chaussee into @vl_Sens 
               
                  select @vl_StatusCh = @@sqlstatus
               end
               
               /*A
               ** Insertion des informations relatives aux stations RADT dans la table
	       ** temporaire TMP_RAD
               */
               
               /*A si autoroute inversee (A57) on met amont et aval dans l'autre sens */
               if exists (select * from CFG..RES_AUT_INV
               			where numero=@vl_Autoroute
               			  and inversee=XDC_PR_INV)
               insert into #TMP_RAD 
               select   @vl_NumSt, @vl_NomServeur, @vl_Liaison, @vl_Rgs, @vl_Seuils, 
               		@vl_Alertes, @vl_Donnees, @vl_NumStAv1, @vl_NumStAm1, @vl_NumStAv2, @vl_NumStAm2,
               		CHA.sens, PNT.boucle, PNT.boucle_inverse, PNT.voie, PNT.poids, @vl_SiteGestion,
			@vl_type, @vl_adresseIP, @vl_port, @vl_idLCR
	       
	          from CFG..RES_CHA CHA, CFG..EQT_PNT_MES PNT
	       
	             where CHA.station 	= @vl_NumSt
	               and PNT.station 	= CHA.station 
	               and PNT.sens     = CHA.sens             		
               else
               insert into #TMP_RAD 
               select   @vl_NumSt, @vl_NomServeur, @vl_Liaison, @vl_Rgs, @vl_Seuils, 
               		@vl_Alertes, @vl_Donnees, @vl_NumStAm1, @vl_NumStAv1, @vl_NumStAm2, @vl_NumStAv2,
               		CHA.sens, PNT.boucle, PNT.boucle_inverse, PNT.voie, PNT.poids, @vl_SiteGestion,
			@vl_type, @vl_adresseIP, @vl_port, @vl_idLCR
	       
	          from CFG..RES_CHA CHA, CFG..EQT_PNT_MES PNT
	       
	             where CHA.station 	= @vl_NumSt
	               and PNT.station 	= CHA.station 
	               and PNT.sens     = CHA.sens             		
               /*A
               ** Remplissage du curseur de la table EQT_RAD
               */
         
               fetch Station into @vl_NumSt, @vl_Rgs, @vl_Liaison, @vl_Donnees, @vl_Seuils, @vl_Alertes , @vl_SiteGestion,
			@vl_type, @vl_adresseIP, @vl_port, @vl_idLCR
               
               select @vl_StatusSt = @@sqlstatus
         
               close Chaussee
            end
            
            
            /*A
            ** Remplissage du curseur de la table EQT_GEN 
            */
                        
            fetch Serveur into @vl_Serveur, @vl_NomServeur
            
            select @vl_StatusSrv = @@sqlstatus
         
         end
         
         /*A
         ** Fermeture des trois curseurs
         */
                  
         close Station
         
         close Serveur
        
         /*A
         ** Renvoi de liste des RADT
         */
         
         select * from #TMP_RAD
             
      end
      
#ifdef CI

      else
      begin   
      
         
         /*A
         ** Recherche du numero de la machine passee en argument
         */
         
         select @vl_Machine = numero from CFT..EQT_GEN
           where type = XDC_EQT_MAC
             and nom  = @va_NomMachine_in
             
        
         /*A
         ** Declaration d'un curseur sur la table EQT_GEN pour la recherche des
         ** serveurs de terminaux
         */
         
         declare Serveur cursor
         for select numero, nom from CFT..EQT_GEN
               where type        = XDC_EQT_SRV
                 and maitre      = @vl_Machine
                 and type_maitre = XDC_MAI_SRV
                   
         open Serveur
         
         /*A
         ** Remplissage du curseur de la table EQT_GEN
         */
         
         fetch Serveur into @vl_Serveur, @vl_NomServeur
         
         select @vl_StatusSrv = @@sqlstatus
         
         /*A
         ** Declaration d'un curseur sur la table EQT_GEN pour la recherche des
         ** stations RADT liees aux differents serveurs de terminaux
         */
         
         declare Station cursor
            for select 	GEN.numero, 
            		RAD.rgs, 
            		RAD.liaison_serie, 
            		RAD.donnees_individuelles, 
            		RAD.seuils, 
            		RAD.alertes ,
			GEN.site_gestion,
			RAD.type,
			RAD.adresse_ip,
			RAD.port,
			RAD.id_lcr
            
                from CFT..EQT_GEN GEN, CFT..EQT_RAD RAD
                  
                  where GEN.type        = XDC_EQT_RAD
                    and GEN.maitre      = @vl_Serveur
                    and GEN.type_maitre = XDC_MAI_RAD
                    and GEN.eqt_supprime = XDC_FAUX
                    and RAD.numero      = GEN.numero
                     
         open Station
         
         /*A
         ** Boucle tant que le curseur de la table EQT_GEN n'est pas vide
         */
         
         while (@vl_StatusSrv = 0)
         begin
         
            /*A
            ** Remplissage du curseur de la table EQT_RAD
            */
         
            fetch Station into @vl_NumSt, @vl_Rgs, @vl_Liaison, @vl_Donnees, @vl_Seuils, @vl_Alertes , @vl_SiteGestion,
			@vl_type, @vl_adresseIP, @vl_port, @vl_idLCR
         
            select @vl_StatusSt = @@sqlstatus
         
            /*A
            ** Declaration d'un curseur sur la table RES_CHA pour la recherche des
            ** chaussee de la station RADT trouvee
            */
            
            declare Chaussee cursor
               for select sens 
            
                  from CFT..RES_CHA
                  
                    where station = @vl_NumSt
         
            /*A
            ** Boucle tant que le curseur de la table EQT_RAD n'est pas vide
            */
         
            while (@vl_StatusSt = 0)
            begin
         
               open Chaussee
         
               /*A
               ** Remplissage du curseur de la table RES_CHA
               */
         
               fetch Chaussee into @vl_Sens 
         
               select @vl_StatusCh = @@sqlstatus
         
               /*A
               ** Boucle tant que le curseur de la table RES_CHA n'est pas vide
               */
         
               select @vl_NumStAm1 = null, @vl_NumStAv1 = null, @vl_NumStAm2 = null, @vl_NumStAv2 = null
               
               while (@vl_StatusCh = 0)
               begin
         
                  /*A
                  ** Recherche du numero de station
                  */
               
                  select @vl_Autoroute = autoroute, @vl_PR = PR 
               
                  from CFT..EQT_GEN 
                 
                    where type        = XDC_EQT_RAD
                      and numero      = @vl_NumSt
                      and maitre      = @vl_Serveur
                      and type_maitre = XDC_MAI_RAD
                                  
                  if @@rowcount != 0
                  begin
               
                     if exists ( select * from CFT..RES_CHA
                                 where station = @vl_NumSt
                                   and sens = @vl_Sens )
                     begin
               
                        if ( ( @vl_Sens = XDC_SENS_SUD ) or ( @vl_Sens = XDC_SENS_NORD ) )
                        begin
                           
                           /*A
                           ** Recherche du numero de la station amont sens SUD ou sens NORD
                           */
               
                           select CHA.PR into #TMP_PR2 from CFT..EQT_GEN GEN, CFT..RES_CHA CHA, CFT..EQT_RAD RAD
                             where GEN.type       = XDC_EQT_RAD
                               and GEN.eqt_supprime = XDC_FAUX
                               and GEN.autoroute  = @vl_Autoroute
                               and RAD.numero	  = GEN.numero
                               and RAD.liaison_serie > 0	  
                               and CHA.station    = GEN.numero
                               and CHA.sens       = @vl_Sens
                               and CHA.PR 	  < @vl_PR 
                    
                           select @vl_PRmax = max(PR) from #TMP_PR2
                      
                           if ( @vl_Sens = XDC_SENS_SUD )
                        
                              select @vl_NumStAm1 = CHA.station from CFT..EQT_GEN GEN, CFT..RES_CHA CHA
                                where GEN.type       = XDC_EQT_RAD
                                  and GEN.eqt_supprime = XDC_FAUX
                                  and GEN.autoroute  = @vl_Autoroute
                                  and CHA.station    = GEN.numero
                                  and CHA.sens       = @vl_Sens	   
                                  and CHA.PR	     = @vl_PRmax
                           else
                              if ( @vl_Sens = XDC_SENS_NORD )
                        
                                 select @vl_NumStAm2 = CHA.station from CFT..EQT_GEN GEN, CFT..RES_CHA CHA
                                   where GEN.type       = XDC_EQT_RAD
                                     and GEN.eqt_supprime = XDC_FAUX
                                     and GEN.autoroute  = @vl_Autoroute
                                     and CHA.station    = GEN.numero
                                     and CHA.sens       = @vl_Sens	   
                                     and CHA.PR	        = @vl_PRmax
                        
                           delete #TMP_PR2
                  
                           /*A
                           ** Recherche du numero de la station aval sens SUD ou sens NORD
                           */
                  
                           insert into #TMP_PR2 select CHA.PR from CFT..EQT_GEN GEN, CFT..RES_CHA CHA, CFT..EQT_RAD RAD
                             where GEN.type       = XDC_EQT_RAD
                               and GEN.eqt_supprime = XDC_FAUX
                               and GEN.autoroute  = @vl_Autoroute
                               and RAD.numero	  = GEN.numero
                               and RAD.liaison_serie > 0	  
                               and CHA.station    = GEN.numero
                               and CHA.sens       = @vl_Sens
                               and CHA.PR 	  > @vl_PR 
                  
                           select @vl_PRmin = min(PR) from #TMP_PR2
                    
                           if ( @vl_Sens = XDC_SENS_SUD )
                        
                              select @vl_NumStAv1 = CHA.station from CFT..EQT_GEN GEN, CFT..RES_CHA CHA
                                where GEN.type       = XDC_EQT_RAD
                                  and GEN.eqt_supprime = XDC_FAUX
                                  and GEN.autoroute  = @vl_Autoroute
                                  and CHA.station    = GEN.numero
                                  and CHA.sens       = @vl_Sens	   
                                  and CHA.PR	     = @vl_PRmin
                           else
                              if ( @vl_Sens = XDC_SENS_NORD )
                        
                                 select @vl_NumStAv2 = CHA.station from CFT..EQT_GEN GEN, CFT..RES_CHA CHA
                                   where GEN.type       = XDC_EQT_RAD
                                     and GEN.eqt_supprime = XDC_FAUX
                                     and GEN.autoroute  = @vl_Autoroute
                                     and CHA.station    = GEN.numero
                                     and CHA.sens       = @vl_Sens	   
                                     and CHA.PR	        = @vl_PRmin
        
                           drop table #TMP_PR2
                  
                        end
                     end
                  end
                  /*A
                  ** Remplissage du curseur de la table EQT_RAD
                  */
         
                  fetch Chaussee into @vl_Sens 
               
                  select @vl_StatusCh = @@sqlstatus
               end
               
               /*A
               ** Insertion des informations relatives aux stations RADT dans la table
	       ** temporaire TMP_RAD
               */
               
               
               /*A si autoroute inversee (A57) on met amont et aval dans l'autre sens */
               if exists (select * from CFT..RES_AUT_INV
               			where numero=@vl_Autoroute
               			  and inversee=XDC_PR_INV)
               insert into #TMP_RAD 
               select   @vl_NumSt, @vl_NomServeur, @vl_Liaison, @vl_Rgs, @vl_Seuils, 
               		@vl_Alertes, @vl_Donnees, @vl_NumStAv1, @vl_NumStAm1, @vl_NumStAv2, @vl_NumStAm2,
               		CHA.sens, PNT.boucle, PNT.boucle_inverse, PNT.voie, PNT.poids, @vl_SiteGestion,
			@vl_type, @vl_adresseIP, @vl_port, @vl_idLCR
	       
	          from CFT..RES_CHA CHA, CFT..EQT_PNT_MES PNT
	       
	             where CHA.station 	= @vl_NumSt
	               and PNT.station 	= CHA.station 
	               and PNT.sens     = CHA.sens             		
               else
               insert into #TMP_RAD 
               select   @vl_NumSt, @vl_NomServeur, @vl_Liaison, @vl_Rgs, @vl_Seuils, 
               		@vl_Alertes, @vl_Donnees, @vl_NumStAm1, @vl_NumStAv1, @vl_NumStAm2, @vl_NumStAv2,
               		CHA.sens, PNT.boucle, PNT.boucle_inverse, PNT.voie, PNT.poids, @vl_SiteGestion,
			@vl_type, @vl_adresseIP, @vl_port, @vl_idLCR
	       
	          from CFT..RES_CHA CHA, CFT..EQT_PNT_MES PNT
	       
	             where CHA.station 	= @vl_NumSt
	               and PNT.station 	= CHA.station 
	               and PNT.sens     = CHA.sens             		
               /*A
               ** Remplissage du curseur de la table EQT_RAD
               */
         
               fetch Station into @vl_NumSt, @vl_Rgs, @vl_Liaison, @vl_Donnees, @vl_Seuils, @vl_Alertes , @vl_SiteGestion,
			@vl_type, @vl_adresseIP, @vl_port, @vl_idLCR
               
               select @vl_StatusSt = @@sqlstatus
         
               close Chaussee
            end
            
            
            /*A
            ** Remplissage du curseur de la table EQT_GEN 
            */
                        
            fetch Serveur into @vl_Serveur, @vl_NomServeur
            
            select @vl_StatusSrv = @@sqlstatus
         
         end
         
         /*A
         ** Fermeture des trois curseurs
         */
                  
         close Station
         
         close Serveur
        
         /*A
         ** Renvoi de liste des RADT
         */
         
         select * from #TMP_RAD
             
      end


#endif      
            
   end
      
   return XDC_OK
         
go  
