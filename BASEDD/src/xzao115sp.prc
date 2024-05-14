/*E*/
/*Fichier : @(#)xzao115sp.prc	1.5      Release : 1.5        Date : 05/11/04
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao115sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao115sp.h" */

/* static char *version = "@(#)xzao115.c	1.1 23/18/94 : xzao115" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Description d'un systeme video
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO115' and type = 'P')
	drop procedure XZAO115
go

create procedure XZAO115
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10),
   @va_Serveur_out		char(10) = null output,
   @va_SysVideo_out		smallint = null output,
   @va_Liaison_out		tinyint = null output,
   @va_Rgs_out			char(3) = null output
   as 

/* 
*
* SP	XZAO115SP
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
* XDY_Eqt	va_Serveur_out
* XDY_Eqt	va_SysVideo_out
* XDY_Octet	va_Liaison_out
* XDT_Rgs	va_Rgs_out
*
* Description d'un systeme video 
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
* volcic	06 Oct 1994	: Creation
* volcic	30 Nov 1994	: Ajout #ifdef CI (v1.2)
* C.T.		19/02/1998	: ajout les colonnes en default suite passage en SQL 11.0.3.2
* Hilmarcher    24/03/2004      : modif type champ code_camera pour dem 270 pilotage cam pc niv 2 (v.1.4)
* JMG           13/05/2005      : modif probleme fichier de conf video genere par teini v1.5-1.6
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Machine		smallint,
   @vl_Serveur		smallint
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null
   or @va_NomMachine_in = null
    
      return XDC_ARG_INV
   
   else   
   
   begin
      
      /*A
      ** Creation d'une table temporaire TMP_VID pour le renvoi de la description 
      ** du systeme video
      */
         
      create table #TMP_VID
      ( type   tinyint  null,
        numLTV smallint null,
        numero smallint null,
        code   char(5) null,
        nom	  char(10) null,
        nblf   tinyint null,
        nblc   tinyint null,
        cycle  bit default 0,
        fixe   bit default 0,
        prepo  bit default 0 )
         
         
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
         ** Recherche du numero de la machine argument
         */
         
         select @vl_Machine = numero from CFG..EQT_GEN
                 where nom  = @va_NomMachine_in
                   and type = XDC_EQT_MAC
                   
         /*A
         ** Recherche du numero du serveur video asscocie � la machine argument
         */
         
         select distinct @va_Serveur_out = GSRV.nom, 
         		@va_SysVideo_out = SYV.numero,
         		@va_Liaison_out  = SYV.liaison_serie,
         		@va_Rgs_out	 = SYV.rgs
         
         from CFG..EQT_GEN GSRV, CFG..EQT_GEN GSYV, 
         	CFG..EQT_SYV SYV
                 
           where GSRV.type	  = XDC_EQT_SRV
             and GSRV.maitre  	  = @vl_Machine
             and GSRV.type_maitre = XDC_MAI_SRV
             and GSYV.type	  = XDC_EQT_SYV
             and GSYV.maitre  	  = GSRV.numero
             and GSYV.type_maitre = XDC_MAI_SYV
             and SYV.numero       = GSYV.numero
            
      	insert into #TMP_VID(type,numLTV,numero,code,nom)
	values (99,@va_Liaison_out,@va_SysVideo_out,@va_Rgs_out,@va_Serveur_out)

         /*A
         ** Recherche de la description du systeme video associes a la machine
         */
         
         select distinct GLTV.type,
         		LTV.numero,
        		LTV.nombre_liaisons_fixes,
        		LTV.nombre_liaisons_cycles
        		
          into #TMP_LTV
        
          from CFG..EQT_GEN GLTV, CFG..EQT_LTV LTV
        	
            where GLTV.type        	= XDC_EQT_LTV
              and GLTV.maitre      	= @va_SysVideo_out
              and GLTV.type_maitre 	= XDC_MAI_LTV
              and LTV.numero       	= GLTV.numero
            
            order by LTV.numero
          
        
         /*A
         ** Recherche de la description des cameras
         */
         
         select distinct GCAM.type,
          		GCAM.maitre,
         		CAM.numero,
        		CAM.code_camera,
        		CAM.cycle_ou_camera,
        		CAM.fixe_ou_mobile,
        		CAM.prepositionnable
        		
          into #TMP_CAM
        
          from CFG..EQT_GEN GCAM, CFG..EQT_CAM CAM
        	
            where GCAM.type        	= XDC_EQT_CAM
              and GCAM.maitre in
                  (select numero from CFG..EQT_GEN
                     where type        	= XDC_EQT_LTV
                       and maitre      	= @va_SysVideo_out
                       and type_maitre 	= XDC_MAI_LTV)
                       
              and GCAM.type_maitre 	= XDC_MAI_CAM
              and CAM.numero       	= GCAM.numero
            
            order by GCAM.maitre, CAM.numero
          
          /*A
          ** Recherche de la description des moniteurs
          */
         
          select distinct GMNT.type,
          		MNT.numero,
			MNT.code_moniteur,
			GMAC.nom
        		
          into #TMP_MNT
        
          from CFG..EQT_GEN GMNT, CFG..EQT_GEN GMAC, CFG..EQT_MNT MNT
        	
            where GMNT.type        	= XDC_EQT_MNT
              and GMNT.maitre      	= @va_SysVideo_out
              and GMNT.type_maitre 	= XDC_MAI_MNT
              and MNT.numero       	= GMNT.numero
              and GMAC.type         	= XDC_EQT_MAC
              and GMAC.numero		= MNT.machine_operateur
          
            order by MNT.numero
          
          /*A
          ** Recherche de la description des magnetoscopes
          */
         
          select distinct GMAG.type,
                        MAG.numero,
        		MAG.code_magnetoscope
        		
          into #TMP_MAG
        
          from CFG..EQT_GEN GMAG, CFG..EQT_MAG MAG
        	
            where GMAG.type         	= XDC_EQT_MAG
              and GMAG.maitre      	= @va_SysVideo_out
              and GMAG.type_maitre 	= XDC_MAI_MAG
              and MAG.numero       	= GMAG.numero
          
           order by MAG.numero
      
         /*A
         ** Insertion de la liste des LTV dans la table temporaire TMP_VID
         */
         
         insert into #TMP_VID (type,numero,nblf,nblc, cycle,fixe,prepo)
         select *,0,0,0 from #TMP_LTV
         
         /*A
         ** Insertion de la liste des cameras dans la table temporaire TMP_VID
         */
         
         insert into #TMP_VID (type,numLTV,numero,code,cycle,fixe,prepo)
         select * from #TMP_CAM
         
         /*A
         ** Insertion de la liste des moniteurs dans la table temporaire TMP_VID
         */
         
         insert into #TMP_VID (type,numero,code,nom,cycle,fixe,prepo)
         select *,0,0,0 from #TMP_MNT
         
         /*A
         ** Insertion de la liste des magnetoscopes dans la table temporaire TMP_VID
         */
         
         insert into #TMP_VID (type,numero,code,cycle,fixe,prepo)
         select *,0,0,0 from #TMP_MAG
          
         /*A
         ** Renvoi de la description du systeme video
         */
         
         select * from #TMP_VID 
      
      end
      
#ifdef CI
      
      else      
      
      begin   
         
         /*A
         ** Recherche du numero de la machine argument
         */
         
         select @vl_Machine = numero from CFT..EQT_GEN
                 where nom  = @va_NomMachine_in
                   and type = XDC_EQT_MAC
                   
         /*A
         ** Recherche du numero du serveur video asscocie � la machine argument
         */
         
         select distinct @va_Serveur_out = GSRV.nom, 
         		@va_SysVideo_out = SYV.numero,
         		@va_Liaison_out  = SYV.liaison_serie,
         		@va_Rgs_out	 = SYV.rgs
         
         from CFT..EQT_GEN GSRV, CFT..EQT_GEN GSYV, 
         	CFT..EQT_SYV SYV
                 
           where GSRV.type	  = XDC_EQT_SRV
             and GSRV.maitre  	  = @vl_Machine
             and GSRV.type_maitre = XDC_MAI_SRV
             and GSYV.type	  = XDC_EQT_SYV
             and GSYV.maitre  	  = GSRV.numero
             and GSYV.type_maitre = XDC_MAI_SYV
             and SYV.numero       = GSYV.numero
            
         select @vl_Serveur,@va_Serveur_out,@va_SysVideo_out,@va_Liaison_out,@va_Rgs_out
        
         /*A
         ** Recherche de la description du systeme video associes a la machine
         */
         
         select distinct GLTV.type,
         		LTV.numero,
        		LTV.nombre_liaisons_fixes,
        		LTV.nombre_liaisons_cycles
        		
          into #TMP_LTV2
        
          from CFT..EQT_GEN GLTV, CFT..EQT_LTV LTV
        	
            where GLTV.type        	= XDC_EQT_LTV
              and GLTV.maitre      	= @va_SysVideo_out
              and GLTV.type_maitre 	= XDC_MAI_LTV
              and LTV.numero       	= GLTV.numero
            
            order by LTV.numero
          
         /*A
         ** Recherche de la description des cameras
         */
         
         select distinct GCAM.type,
          		GCAM.maitre,
         		CAM.numero,
        		CAM.code_camera,
        		CAM.cycle_ou_camera,
        		CAM.fixe_ou_mobile,
        		CAM.prepositionnable
        		
          into #TMP_CAM2
        
          from CFT..EQT_GEN GCAM, CFT..EQT_CAM CAM
        	
            where GCAM.type        	= XDC_EQT_CAM
              and GCAM.maitre in
                  (select numero from CFT..EQT_GEN
                     where type        	= XDC_EQT_LTV
                       and maitre      	= @va_SysVideo_out
                       and type_maitre 	= XDC_MAI_LTV)
                       
              and GCAM.type_maitre 	= XDC_MAI_CAM
              and CAM.numero       	= GCAM.numero
            
            order by GCAM.maitre, CAM.numero
          
          /*A
          ** Recherche de la description des moniteurs
          */
         
          select distinct GMNT.type,
          		MNT.numero,
			MNT.code_moniteur,
			GMAC.nom
        		
          into #TMP_MNT2
        
          from CFT..EQT_GEN GMNT, CFT..EQT_GEN GMAC, CFT..EQT_MNT MNT
        	
            where GMNT.type        	= XDC_EQT_MNT
              and GMNT.maitre      	= @va_SysVideo_out
              and GMNT.type_maitre 	= XDC_MAI_MNT
              and MNT.numero       	= GMNT.numero
              and GMAC.type         	= XDC_EQT_MAC
              and GMAC.numero		= MNT.machine_operateur
          
            order by MNT.numero
          
          /*A
          ** Recherche de la description des magnetoscopes
          */
         
          select distinct GMAG.type,
                        MAG.numero,
        		MAG.code_magnetoscope
        		
          into #TMP_MAG2
        
          from CFT..EQT_GEN GMAG, CFT..EQT_MAG MAG
        	
            where GMAG.type         	= XDC_EQT_MAG
              and GMAG.maitre      	= @va_SysVideo_out
              and GMAG.type_maitre 	= XDC_MAI_MAG
              and MAG.numero       	= GMAG.numero
          
           order by MAG.numero
      
         /*A
         ** Insertion de la liste des LTV dans la table temporaire TMP_VID
         */
         
         insert into #TMP_VID (type,numero,nblf,nblc, cycle,fixe,prepo)
         select *,0,0,0 from #TMP_LTV2
         
         /*A
         ** Insertion de la liste des cameras dans la table temporaire TMP_VID
         */
         
         insert into #TMP_VID (type,numLTV,numero,code,cycle,fixe,prepo)
            select * from #TMP_CAM2
         
         /*A
         ** Insertion de la liste des moniteurs dans la table temporaire TMP_VID
         */
         
         insert into #TMP_VID (type,numero,code,nom,cycle,fixe,prepo)
            select *,0,0,0 from #TMP_MNT2
         
         /*A
         ** Insertion de la liste des magnetoscopes dans la table temporaire TMP_VID
         */
         
         insert into #TMP_VID (type,numero,code,cycle,fixe,prepo)
          select *,0,0,0 from #TMP_MAG2
          
         /*A
         ** Renvoi de la description du systeme video
         */
         
         select * from #TMP_VID 
      
      end
#endif      
      
   end
      
   return XDC_OK
         
go    
