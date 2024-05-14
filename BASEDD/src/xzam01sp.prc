/*E*/
/*Fichier : @(#)xzam01sp.prc	1.14      Release : 1.14        Date : 01/15/10
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAM * FICHIER xzam01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alarmes >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic		03 Nov 1994	: Creation
* gaborit		05 jan 1994	: Modif vl_horodate (1.2)
* gaborit       24 jul 1995	: mise a jour de date 1er disparition (1.3)
* gaborit       02 oct 1995	: modifs verification double (1.4)
* torregrossa   21 Nov 1995	: Pas de creation d'alarme sur un equipement HS (1.5)
* torregrossa	13 Fev 1996	: Reveils sur equipement remis en dispo (1.6)
* torregrossa	04 Sep 1996	: Suppression des reveils si PC simplifie (1.7)
*                                 ajout test sur DC et DS
* C.P.			07/01/98	: Correction gestion multi-sites dans 
*				  les procs (V 1.) (dem / 1536)
* C.T.			30/01/98	: Correction gestion multi-sites pour tasrv (1536) V1.9
* LCL           09/07/07        : Suppression DY V1.10
* JBL			17/10/08		: passage de tinyint vers smallint pour le type d'alarme (va_Type_in)
* JMG			15/09/09	: FACTIS DEM914
* JMG			15/01/10	: correction retour au vert intempestif FACTIS DEM914
* VR			14/12/11 	: Ajout PRV (DEM/1016)
* JMG			06/09/16	: FACTIS 1.15
* JMG   		09/03/17	: REGIONALISATION - DEM1220 1.18
* GGY			26/09/23	: Ajout district CA (DEM-483)
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*   Cree une nouvelle fiche alarme, ou met a jour celle existant deja
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAM;01
   @va_Horodate_in	datetime = null,
   @va_TypeEqt_in	tinyint = null,
   @va_Eqt_in		smallint = null,
   @va_Type_in		smallint = null,
   @va_Etat_in		bit
   as 

/* 
*
* SP	XZAM01SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Horodate		va_Horodate_in
* XDY_TypeEqt		va_TypeEqt_in
* XDY_Eqt		va_Eqt_in
* XDY_TypeAlarme	va_Type_in
* XDY_Booleen		va_Etat_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Cree une nouvelle fiche alarme, ou met a jour celle existant deja
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
* 6	XDC_ACT_REFUSEE : pas de creation sur equipement HS
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* recherche dans la table ALARME s'il existe deja une fiche alarme pour le meme
* equipement et le meme type d'alarme.
*
* Si c'est le cas, met a jour cette fiche (etat, horodate dernier changement, horodate
* premiere disparition, nombre dapparitions), sinon, cree une nouvelle fiche
*
* Met a jour l'indisponibilite de l'equipement en fonction du type d'alarme (majeure,
* mineure, critique) et de la desactivation eventuelle + appel ATRP18 (XZIS02)
*
* Incremente le compteur globlal d'alarmes de l'equipement
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Horodate		datetime,
   @vl_NbAlrm		smallint,
   @vl_TypeAlrm		smallint,
   @vl_TypeCr 		bit, 
   @vl_TypeMj 		bit, 
   @vl_TypeMn 		bit,
   @vl_Changt 		smallint,
   @vl_NomMaitre 	char(10), 
   @vl_EtatEqt 		smallint,
   @vl_NumEqt 		smallint, 
   @vl_TypeEqt 		tinyint, 
   @vl_Status 		int,
   @vl_NumMaitre 	smallint, 
   @vl_TypeMaitre 	tinyint, 
   @vl_MachineTrouvee 	bit, 
   @vl_Site 		tinyint, 
   @vl_Critique 	bit, 
   @vl_Desactive 	bit, 
   @vl_Majeure 		bit, 
   @vl_Mineure 		bit,
   @vl_HS 		bit, 
   @vl_Inhibe 		bit, 
   @vl_Erreur 		bit,
   @vl_Evt 		int, 
   @vl_Cle 		tinyint, 
   @vl_Autoroute	T_AUTOROUTE, 
   @vl_PR 		T_PR, 
   @vl_Sens 		T_SENS, 
   @vl_Intitule		varchar(50), 
   @vl_Texte		char(100) ,
   @vl_nb_crit          int,
   @vl_animation        int,
   @vl_Critique_tmp     bit
   
     
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   select @vl_TypeCr = XDC_FAUX, 
          @vl_TypeMj = XDC_FAUX, 
          @vl_TypeMn = XDC_FAUX,
          @vl_Critique = XDC_FAUX, 
          @vl_Desactive = XDC_FAUX, 
          @vl_Majeure = XDC_FAUX, 
          @vl_Mineure = XDC_FAUX,
          @vl_HS = XDC_FAUX, 
          @vl_Inhibe = XDC_FAUX, 
          @vl_Erreur = XDC_FAUX,
	  @vl_nb_crit = 0,
	  @vl_Critique_tmp = XDC_FAUX
          
          
   
   if @va_Horodate_in = null
   or @va_TypeEqt_in = null
   or @va_Eqt_in = null
   or @va_Type_in = null
   or @va_Etat_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   begin
   
   /*A verification de l'existence de l'equipement */
   
       if not exists (select * from CFG..EQT_GEN where numero=@va_Eqt_in and type=@va_TypeEqt_in)
       return XDC_ARG_INV
       
   /*A verification de l'existence de l'alarme */
   
       if not exists (select * from CFG..TYP_ALM where numero=@va_Type_in)
       return XDC_ARG_INV
       
   /*A verification du site */
   
       select @vl_Site=sit from EXP..EQT_DSP where equipement=@va_Eqt_in and type=@va_TypeEqt_in
        and dernier = 1

       if not exists (select serveur from CFG..RES_DIS 
				where numero = @vl_Site and serveur = @@servername)
	  return ( XDC_ARG_INV )
       
       
   /*A
   ** Si les parametres sont non nuls
   */
         
      if exists (select * from EXP..ALM_EQT
        where type 		= @va_Type_in
          and type_equipement 	= @va_TypeEqt_in
          and equipement	= @va_Eqt_in)
          
      /*A
      ** La fiche alarme existe
      ** Update de la fiche alarme
      */
      
      begin
      
         if @va_Etat_in = XDC_VRAI
         begin
            
            update EXP..ALM_EQT
         
            set date_dernier_changement = @va_Horodate_in,
         	etat			= @va_Etat_in,
         	nombre_d_apparitions 	= nombre_d_apparitions + 1 
         
            where type 		 = @va_Type_in
             and type_equipement = @va_TypeEqt_in
             and equipement	 = @va_Eqt_in
          
         end
          
         else
            
            update EXP..ALM_EQT
         
            set date_dernier_changement = @va_Horodate_in,
         	etat			= @va_Etat_in,
		date_1er_disparition	= isnull(date_1er_disparition,@va_Horodate_in)
            
            where type 		  = @va_Type_in
              and type_equipement = @va_TypeEqt_in
              and equipement	  = @va_Eqt_in
          
      end
          
      else
      
      /*A
      ** La fiche alarme n'existe pas
      ** Creation de la fiche alarme
      */
      
      begin               
            
         if @va_Etat_in = XDC_VRAI
         begin
         
	    /*B Si l'equipement est HS alors la creation de l'alarme n'est pas effectu�e */
	    if @va_Eqt_in is not null and exists (select * from EXP..EQT_DSP 
						where equipement = @va_Eqt_in and type = @va_TypeEqt_in and 
							(HS = XDC_VRAI or inhibe = XDC_VRAI) and dernier = XDC_VRAI)
		return XDC_ACT_REFUSEE

            insert into EXP..ALM_EQT ( type, type_equipement, equipement, date_1er_apparition, nombre_d_apparitions, etat, sit )
            values ( @va_Type_in, @va_TypeEqt_in, @va_Eqt_in, @va_Horodate_in, 1, @va_Etat_in, @vl_Site )  
         
         end
         
         else
         
            return XDC_OK
      end
      
      
      /*A mise a jour du nombre total d'apparitions */
      if not exists (select * from EXP..ALM_NBR
                     where type=@va_Type_in
                       and type_equipement=@va_TypeEqt_in
                       and equipement=@va_Eqt_in)
      
            insert into EXP..ALM_NBR 
            values ( @va_Type_in, @va_TypeEqt_in, @va_Eqt_in, 1, @vl_Site )
      else
      begin
         if @va_Etat_in = XDC_VRAI
      
            update EXP..ALM_NBR
         
            set nombre_d_apparitions = nombre_d_apparitions + 1 
         
            where type 		  = @va_Type_in
              and type_equipement = @va_TypeEqt_in
              and equipement	  = @va_Eqt_in
      end
           
         
      /*A
      ** Recherche des caracteristiques du type de l'alarme
      */
         
      select @vl_TypeCr      = critique,
	      @vl_TypeMj      = majeure,
	      @vl_TypeMn      = mineure

	       from CFG..TYP_ALM

	where numero  = @va_Type_in

      select @vl_Critique_tmp = @vl_TypeCr

      if @va_Etat_in = XDC_FAUX
           
         select @vl_TypeCr 	= XDC_FAUX, 
        	@vl_TypeMj 	= XDC_FAUX, 
        	@vl_TypeMn 	= XDC_FAUX  
         
      
        /*A
        ** Recherche d'une eventuelle fiche alarme associee a l'equipement autre que
        ** celle en exploitation
        */
        
        select @vl_TypeCr = TYP.critique
        
        from EXP..ALM_EQT ALM, CFG..TYP_ALM TYP
          
          where ALM.type_equipement  	 = @va_TypeEqt_in
            and ALM.equipement	 	 = @va_Eqt_in
            and ALM.type 		!= @va_Type_in
            and TYP.numero		 = ALM.type
            and TYP.critique 		 = XDC_VRAI
            and ALM.etat		 = XDC_VRAI
        
        if @vl_TypeCr = XDC_FAUX
        
           select @vl_TypeMj = TYP.majeure
        
           from EXP..ALM_EQT ALM, CFG..TYP_ALM TYP
          
           where ALM.type_equipement  	 = @va_TypeEqt_in
             and ALM.equipement	 	 = @va_Eqt_in
             and ALM.type 		!= @va_Type_in
             and TYP.numero		 = ALM.type
             and TYP.majeure 		 = XDC_VRAI
             and ALM.etat		 = XDC_VRAI
        
        else
        
        if @vl_TypeMj = XDC_FAUX
        
           select @vl_TypeMn = TYP.mineure
        
           from EXP..ALM_EQT ALM, CFG..TYP_ALM TYP
          
           where ALM.type_equipement  	 = @va_TypeEqt_in
             and ALM.equipement	 	 = @va_Eqt_in
             and ALM.type 		!= @va_Type_in
             and TYP.numero		 = ALM.type
             and TYP.mineure 		 = XDC_VRAI
             and ALM.etat		 = XDC_VRAI
        
        
        /*A 
	** Recherche le site 
	*/
	
	select  @vl_Site 	= EXP..EQT_DSP.sit ,
		@vl_Critique 	= EXP..EQT_DSP.critique,
		@vl_Desactive 	= EXP..EQT_DSP.desactive,
		@vl_Majeure 	= EXP..EQT_DSP.majeure,
		@vl_Inhibe 	= EXP..EQT_DSP.inhibe,
		@vl_Mineure 	= EXP..EQT_DSP.mineure,
		@vl_HS 		= EXP..EQT_DSP.HS
	
	from EXP..EQT_DSP
	
	where	equipement = @va_Eqt_in and 
		type 	= @va_TypeEqt_in and
		dernier 	= XDC_OUI

	if @@rowcount <>1
	begin
		select  @vl_Site 	= EXP..EQT_DSP.sit ,
			@vl_Critique 	= EXP..EQT_DSP.critique,
			@vl_Desactive 	= EXP..EQT_DSP.desactive,
			@vl_Majeure 	= EXP..EQT_DSP.majeure,
			@vl_Inhibe 	= EXP..EQT_DSP.inhibe,
			@vl_Mineure 	= EXP..EQT_DSP.mineure,
			@vl_HS 		= EXP..EQT_DSP.HS
	
		from EXP..EQT_DSP
	
		where	equipement = @va_Eqt_in and 
			type 	= @va_TypeEqt_in
		having	horodate=max(horodate) and
			equipement = @va_Eqt_in and 
			type 	= @va_TypeEqt_in
		
		if @@rowcount = 0
		return XDC_NOK
	end

	begin
		
		/*A 
		** Positionner les indicateurs de l'equipement 
		*/
		select @vl_Horodate=getdate()
		
		insert EXP..EQT_DSP ( equipement, type,  sit, critique, desactive, majeure, mineure, 
					HS, inhibe, horodate, dernier)
		values( @va_Eqt_in, @va_TypeEqt_in, @vl_Site, @vl_TypeCr, @vl_Desactive, @vl_TypeMj, @vl_TypeMn, 
					@vl_HS, @vl_Inhibe,@vl_Horodate , XDC_VRAI )
	
		if @@rowcount != 1
			select @vl_Erreur = XDC_OUI
		else	
		begin	
			/*A 
			** Positionner l'indicateur de dernier enregistrement a 0 pour 
			** les enregistrements precedents pour cet equipement          
			*/
			
			update EXP..EQT_DSP 
			
			set dernier = XDC_NON
			
			where	equipement = @va_Eqt_in and type = @va_TypeEqt_in and
				horodate != @vl_Horodate and dernier = XDC_OUI

#ifndef PC_SIMPL
			/*A
			** declenchement d'un reveil si l'equipement est redevenu disponible a la fin d'alarme et
			** si l'equipement est un PMV ou un BRA ou un TUBE ou un echangeur 
			*/
			if @va_Etat_in = XDC_FAUX and (@va_TypeEqt_in = XDC_EQT_PMV or @va_TypeEqt_in = XDC_EQT_NAV or
			   @va_TypeEqt_in = XDC_EQT_TUBE or @va_TypeEqt_in = XDC_EQT_ECH or @va_TypeEqt_in = XDC_EQT_PMVA or @va_TypeEqt_in = XDC_EQT_PRV)

				if @vl_Desactive = XDC_FAUX and @vl_Inhibe = XDC_FAUX and
				   @vl_HS = XDC_FAUX and @vl_TypeCr = XDC_FAUX and @vl_TypeMj = XDC_FAUX

					if @vl_Majeure = XDC_VRAI or @vl_Critique = XDC_VRAI
					begin
						/* rechercher si des fmc sont en attente de cet equipement */
						/* et on selectionne la plus prioritaire */
						select @vl_Evt = null, @vl_Cle = null
						set rowcount 1
						select
							@vl_Evt = evenement,
							@vl_Cle = cle
						from EXP..EQT_FIL
						where	equipement = @va_Eqt_in and 
							type_equipement = @va_TypeEqt_in
						order by priorite desc, distance asc

						set rowcount 0

						if @vl_Evt is not null and @vl_Cle is not null
						begin
							select 
								@vl_Autoroute = autoroute,
								@vl_PR = PR,
								@vl_Sens = sens
							from CFG..EQT_GEN
							where numero = @va_Eqt_in and type = @va_TypeEqt_in
	
							/* nommage de l'equipement */
							exec XZAC;60	@va_TypeEqt_in, @vl_Autoroute, @vl_PR, @vl_Sens, 
									@vl_Intitule output
	
							/* envoi du reveil sur la fmc concernee */
							select @vl_Texte=XDC_REV_DSP_EQT + @vl_Intitule + XDC_REV_DSP_EQT_SUITE
							exec PRC..XZAR;03 @vl_Evt,@vl_Cle,null,@vl_Texte,@vl_Site
						end
					end
#endif
		end
	end

	if @vl_Erreur = XDC_OUI
		return XDC_NOK

	/*A Initialisation des variables pour la recherche de la 
	**  machine pilotant l'equipement                        
	*/
	
	select	@vl_NumEqt 		= @va_Eqt_in, 
		@vl_TypeEqt 		= @va_TypeEqt_in,
		@vl_MachineTrouvee 	= XDC_NON

	/*A 
	** Recherche de la machine pilotant l'equipement 
	*/
	
	while @vl_MachineTrouvee = XDC_NON
	begin
		/*B 
		** Recherche la machine pilotant un equipement 
		*/
		
		select  @vl_NumMaitre 	= maitre,
			@vl_TypeMaitre 	= type_maitre,
			@vl_NomMaitre 	= nom
		
		from CFG..EQT_GEN
		
		where numero = @vl_NumEqt and type = @vl_TypeEqt

		if @@rowcount != 1
			return XDC_NOK

		/*B 
		** Test si l'equipement n'a plus de maitre 
		*/
		
		if @vl_NumMaitre = null or @vl_TypeMaitre = XDC_EQT_MAC
			select @vl_MachineTrouvee = XDC_OUI
		
		else	
			select @vl_NumEqt = @vl_NumMaitre, @vl_TypeEqt = @vl_TypeMaitre
	
	end
		
	
	/*A 
	** Initialiser le changement d'etat de l'equipement 
	*/
	select @vl_Changt = 0
	
	if @vl_TypeCr != @vl_Critique
	   
	   select @vl_Changt = XDC_EQT_CRITIQUE
	
	/*disparition d'une alarme critique*/
	if @vl_Critique_tmp = XDC_VRAI and @va_Etat_in = XDC_FAUX
	begin
		select @vl_nb_crit = count(*) from CFG..TYP_ALM, EXP..ALM_EQT
		where CFG..TYP_ALM.numero = EXP..ALM_EQT.type
		and etat = XDC_VRAI
		and critique = XDC_VRAI
	end
	if @va_Etat_in = XDC_VRAI
		select  @vl_nb_crit = 1

	if @vl_TypeMj != @vl_Majeure
	   
	   select @vl_Changt = @vl_Changt + XDC_EQT_MAJEUR
	
	if @vl_TypeMn != @vl_Mineure
	   
	   select @vl_Changt = @vl_Changt + XDC_EQT_MINEUR
	
		
	/*A 
	** Initialiser l'etat de l'equipement a positionner 
	*/
	
	select	@vl_EtatEqt =	XDC_EQT_HS * @vl_HS + 
				XDC_EQT_DESACTIVE * @vl_Desactive + 
				XDC_EQT_MINEUR * @vl_TypeMn + 
				XDC_EQT_INHIBE * @vl_Inhibe +
				XDC_EQT_CRITIQUE * @vl_TypeCr + 
				XDC_EQT_MAJEUR * @vl_TypeMj
				
	/*A 
	** Desactivation de l'equipement 
	*/
	
	if @vl_NumMaitre != null
	begin
		if @vl_Site = XDC_CI
		begin
			exec @vl_Status = TASRV_CI...ATRP18 @vl_NomMaitre, @va_TypeEqt_in, @va_Eqt_in, @vl_Changt, @vl_EtatEqt
			if @vl_TypeCr != @vl_Critique and @va_Etat_in = XDC_VRAI
			/*alarme critique qui apparait*/
			begin
      				if not exists (select * from EXP..ALM_IHM
        				where type 		= @va_Type_in
          				and type_equipement 	= @va_TypeEqt_in
          				and equipement	= @va_Eqt_in)
         			begin
            				insert into EXP..ALM_IHM ( type, type_equipement, equipement )
            				values ( @va_Type_in, @va_TypeEqt_in, @va_Eqt_in )  
				end
				select @vl_animation = 1
				exec @vl_Status = TASRV_CI...ATRP19 @vl_NomMaitre, @va_TypeEqt_in, @va_Eqt_in, @vl_Changt, @vl_EtatEqt, @vl_animation
			end
			else
			if @vl_Critique_tmp = XDC_VRAI
			/*alarme critique qui disparait*/
			begin
				delete EXP..ALM_IHM
				where type              = @va_Type_in
				and type_equipement     = @va_TypeEqt_in
				and equipement  = @va_Eqt_in

				select @vl_nb_crit=count(*) from EXP..ALM_IHM
				if @vl_nb_crit=0
				begin
					select @vl_animation=0
					select @vl_Changt = XDC_EQT_CRITIQUE
					select @vl_EtatEqt = XDC_EQT_CRITIQUE
					exec @vl_Status = TASRV_CI...ATRP19 @vl_NomMaitre, @va_TypeEqt_in, @va_Eqt_in, @vl_Changt, @vl_EtatEqt, @vl_animation
				end		
			end
		end
		else 
		
		if @vl_Site = XDC_CA
		begin
			exec @vl_Status = TASRV_CA...ATRP18 @vl_NomMaitre, @va_TypeEqt_in, @va_Eqt_in, @vl_Changt, @vl_EtatEqt
			if @vl_TypeCr != @vl_Critique and @va_Etat_in = XDC_VRAI
			/*alarme critique qui apparait*/
			begin
      				if not exists (select * from EXP..ALM_IHM
        				where type 		= @va_Type_in
          				and type_equipement 	= @va_TypeEqt_in
          				and equipement	= @va_Eqt_in)
         			begin
            				insert into EXP..ALM_IHM ( type, type_equipement, equipement )
            				values ( @va_Type_in, @va_TypeEqt_in, @va_Eqt_in )  
				end
				select @vl_animation = 1
				exec @vl_Status = TASRV_CA...ATRP19 @vl_NomMaitre, @va_TypeEqt_in, @va_Eqt_in, @vl_Changt, @vl_EtatEqt, @vl_animation
			end
			else
			if @vl_Critique_tmp = XDC_VRAI
			/*alarme critique qui disparait*/
			begin
				delete EXP..ALM_IHM
				where type              = @va_Type_in
				and type_equipement     = @va_TypeEqt_in
				and equipement  = @va_Eqt_in

				select @vl_nb_crit=count(*) from EXP..ALM_IHM
				if @vl_nb_crit=0
				begin
					select @vl_animation=0
					select @vl_Changt = XDC_EQT_CRITIQUE
					select @vl_EtatEqt = XDC_EQT_CRITIQUE
					exec @vl_Status = TASRV_CA...ATRP19 @vl_NomMaitre, @va_TypeEqt_in, @va_Eqt_in, @vl_Changt, @vl_EtatEqt, @vl_animation
				end		
			end
		end
		else 

		if @vl_Site = XDC_VC
		begin
			exec @vl_Status = TASRV_VC...ATRP18 @vl_NomMaitre, @va_TypeEqt_in, @va_Eqt_in, @vl_Changt, @vl_EtatEqt
			/*alarme critique qui apparait*/
			if @vl_TypeCr != @vl_Critique and @va_Etat_in = XDC_VRAI
			begin
      				if not exists (select * from EXP..ALM_IHM
        				where type 		= @va_Type_in
          				and type_equipement 	= @va_TypeEqt_in
          				and equipement	= @va_Eqt_in)
         			begin
            				insert into EXP..ALM_IHM ( type, type_equipement, equipement )
            				values ( @va_Type_in, @va_TypeEqt_in, @va_Eqt_in )  
				end
				select @vl_animation = 1
				exec @vl_Status = TASRV_VC...ATRP19 @vl_NomMaitre, @va_TypeEqt_in, @va_Eqt_in, @vl_Changt, @vl_EtatEqt, @vl_animation
			end
			/*alarme critique qui disparait*/
			else 
			if @vl_Critique_tmp = XDC_VRAI
			begin
				delete EXP..ALM_IHM
				where type              = @va_Type_in
				and type_equipement     = @va_TypeEqt_in
				and equipement  = @va_Eqt_in

				select @vl_nb_crit=count(*) from EXP..ALM_IHM
				if @vl_nb_crit = 0
				begin
					select @vl_animation=0
					select @vl_Changt = XDC_EQT_CRITIQUE
					select @vl_EtatEqt = XDC_EQT_CRITIQUE
					exec @vl_Status = TASRV_VC...ATRP19 @vl_NomMaitre, @va_TypeEqt_in, @va_Eqt_in, @vl_Changt, @vl_EtatEqt, @vl_animation
				end
			end		
		end
		else	
		if @vl_Site = XDC_DP
		begin
			exec @vl_Status = TASRV_DP...ATRP18 @vl_NomMaitre, @va_TypeEqt_in, @va_Eqt_in, @vl_Changt, @vl_EtatEqt
			if @vl_TypeCr != @vl_Critique and @va_Etat_in = XDC_VRAI
			begin
				select @vl_animation = 1
				exec @vl_Status = TASRV_DP...ATRP19 @vl_NomMaitre, @va_TypeEqt_in, @va_Eqt_in, @vl_Changt, @vl_EtatEqt, @vl_animation
			end
			else
			if @vl_Critique_tmp = XDC_VRAI
			begin
				if @vl_nb_crit=0
				begin
					select @vl_animation=0
					select @vl_Changt = XDC_EQT_CRITIQUE
					select @vl_EtatEqt = XDC_EQT_CRITIQUE
					exec @vl_Status = TASRV_DP...ATRP19 @vl_NomMaitre, @va_TypeEqt_in, @va_Eqt_in, @vl_Changt, @vl_EtatEqt, @vl_animation
				end		
			end
		end
		else	
		if @vl_Status != XDC_OK
			return @vl_Status
	end
      
      
      
   return (XDC_OK)

   end

go  
