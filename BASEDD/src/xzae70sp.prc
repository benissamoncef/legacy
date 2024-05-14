/*E*/
/*  Fichier : $Id: xzae70sp.prc,v 1.10 1998/09/22 13:27:56 gaborit Exp $      Release : $Revision: 1.10 $        Date : $Date: 1998/09/22 13:27:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae70sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	01/09/94	: Creation
* C.T.	13/09/94	: Modif sur test de tete de bouchon
* C.T.  04/10/94        : Ajout de la suppression de bouclage 
*			  et la colonne spid dans LISTE_CONSEQ
*                         (V 1.5)
* C.T.	10/11/94	: Modif bugs applix (V 1.6)
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.7)
* C.T.+BG 02/10/95	: ajout condition sur spid dans select final (1.8)
* C.T.	02/04/98	: Modif suite passage en SQL server 11.0.3 (1.9)
* C.P.	17/06/98	: Modif separation EXP et HIS (V 1.10)
* LCL	03/05/23	: Attente remontee replication pour COCKPIT
* LCL	15/05/23	: Adaptation attente remontee replication COCKPIT
* LCL	31/05/23	: Suppression attente remontee replication COCKPIT
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne la liste des consequences d'un evenement
* 
* Sequence d'appel
* SP	XZAE70_ListeConsequence
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Horodate		va_Horodate_in 
* XDY_Booleen		va_Memorisation_in 
* 
* Arguments en sortie
* XDY_Entier		va_NumEvt_out
* XDY_Octet		va_CleEvt_out
* XDY_Octet		va_Type_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de fiche main courante
* XDC_ARG_INV
* <0 erreur sybase 
* 
* Conditions d'utilisation
* Attention, saute la queue du bouchon
* les arguments de sortie sont retournes dans une liste
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* SI Type(NumEvt)=TetedeBouchon ALORS NumEvt=NumQueue
* Select numero, cle, type from EVENEMENT where cause, cle_cause=NumEvt
---------------------------------------------------*/

use PRC
go


create procedure XZAE;70
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_Horodate_in		datetime = null,
	@va_Memorisation_in	bit
as
	declare @vl_NumEvt int, @vl_CleEvt tinyint,  @vl_Classe tinyint, 
	@vl_TeteBouchon bit, @vl_Status int, @vl_replication_ok int, @vl_timeout int

	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A initialisation variable locale */
	select @vl_TeteBouchon = XDC_NON

	/*A controle les parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or @va_Horodate_in = null
		return XDC_ARG_INV

#ifdef HIST
	/*! cas ou la procedure est creee sur HIST */

	/*A liste temporaire des evenements de niveau 0 */
	create table #LISTE (numero int, cle tinyint)
	
	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
		/*A recherche du type de l'evenement */
		select @vl_Classe = CFG..TYP_FMC.classe from HIS..FMC_GEN, CFG..TYP_FMC
		where	HIS..FMC_GEN.numero = @va_NumEvt_in and 
			HIS..FMC_GEN.cle = @va_CleEvt_in and
			CFG..TYP_FMC.numero = HIS..FMC_GEN.type
	
		/*A liste des consequences de l'evenement */
		/*! table non utilisee dans ce cas */
		if @@rowcount != 1
			return	XDC_NOK
	
		/*A Test si c'est une tete de bouchon */
		if @vl_Classe = XZAEC_ClasseGenerique 
		begin
			/*B recherche les queues de bouchon : la consequence queue */
			/*B de bouchon est sautee                                  */
			insert #LISTE( numero , cle )
			select  HIS..FMC_GEN.numero ,  HIS..FMC_GEN.cle 
			from HIS..FMC_GEN, CFG..TYP_FMC
			where	HIS..FMC_GEN.cause = @va_NumEvt_in and 
				HIS..FMC_GEN.cle_cause = @va_CleEvt_in  and
				CFG..TYP_FMC.classe = XZAEC_ClasseBouchon and 
				CFG..TYP_FMC.numero = HIS..FMC_GEN.type and 
				HIS..FMC_GEN.horodate_lien_cause <= @va_Horodate_in
	
			if @@rowcount > 0
				select @vl_TeteBouchon = XDC_OUI
		end
	
		if @vl_TeteBouchon = XDC_NON
		begin
			/*B cas d'evenement pas de type tete de bouchon */
			insert #LISTE 
			values (@va_NumEvt_in, @va_CleEvt_in )
		end
	
		/*A creation d'un curseur pour parcourir la liste */
		declare Pointeur cursor
		for select numero, cle
		from #LISTE
		open Pointeur
	
		/* parcours de la liste */
		fetch Pointeur into @vl_NumEvt, @vl_CleEvt
		while (@@sqlstatus = 0)
		begin
			/*A liste des consequences de l'evenement en enlevant */
			/*A les bouclages possibles                           */
			insert LISTE_CONSEQ ( numero , cle , type , numero_pere , cle_pere, spid )
			select
				HIS..FMC_GEN.numero , 
				HIS..FMC_GEN.cle, 
				HIS..FMC_GEN.type,
				@va_NumEvt_in,
				@va_CleEvt_in,
				@@spid
			from HIS..FMC_GEN
			where	HIS..FMC_GEN.cause =  @vl_NumEvt and 
				HIS..FMC_GEN.cle_cause = @vl_CleEvt and 
				HIS..FMC_GEN.horodate_lien_cause <= @va_Horodate_in and
				not exists ( select * from LISTE_CONSEQ, HIS..FMC_GEN
					     where  HIS..FMC_GEN.cause =  @vl_NumEvt and
						HIS..FMC_GEN.cle_cause = @vl_CleEvt and
						HIS..FMC_GEN.horodate_lien_cause <= @va_Horodate_in and
						LISTE_CONSEQ.numero = HIS..FMC_GEN.numero and
						LISTE_CONSEQ.cle = HIS..FMC_GEN.cle and
						LISTE_CONSEQ.spid = @@spid)
		
			/*B evenement suivant  de niveau 0 */
			fetch Pointeur into @vl_NumEvt, @vl_CleEvt
		end
	

#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */
	
	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAE;70
			@va_NumEvt_in, @va_CleEvt_in, @va_Horodate_in, @va_Memorisation_in
 		if @vl_Status = null
                       		return XDC_PRC_INC
                else return @vl_Status
	end
	else
	begin
		create table #LISTE (numero int, cle tinyint)

		/* Adaptation PIT pour attente remontee replication */
/*		select @vl_replication_ok=0
		select @vl_timeout=0
		while ( @vl_replication_ok = 0 and @vl_timeout < 50 )
		begin
			select @vl_timeout=@vl_timeout+1
			select @vl_NumEvt = numero from EXP..FMC_GEN
			where EXP..FMC_GEN.numero = @va_NumEvt_in and
				EXP..FMC_GEN.cle = @va_CleEvt_in

			if @@rowcount > 0
				select @vl_replication_ok = 1
			else
				waitfor delay "00:00:00.100"
		end
*/
		/*A recherche du type de l'evenement */
		select @vl_Classe = CFG..TYP_FMC.classe from EXP..FMC_GEN, CFG..TYP_FMC
		where	EXP..FMC_GEN.numero = @va_NumEvt_in and 
			EXP..FMC_GEN.cle = @va_CleEvt_in and
			CFG..TYP_FMC.numero = EXP..FMC_GEN.type
	
		/*A liste des consequences de l'evenement */
		/*! table non utilisee dans ce cas */
		if @@rowcount != 1
			return	XDC_NOK
	
		/*A Test si c'est une tete de bouchon */
		if @vl_Classe = XZAEC_ClasseGenerique 
		begin
			/*B recherche les queues de bouchon : la consequence queue */
			/*B de bouchon est sautee                                  */
			insert #LISTE( numero , cle )
			select  EXP..FMC_GEN.numero ,  EXP..FMC_GEN.cle 
			from EXP..FMC_GEN, CFG..TYP_FMC
			where	EXP..FMC_GEN.cause = @va_NumEvt_in and 
				EXP..FMC_GEN.cle_cause = @va_CleEvt_in  and
				CFG..TYP_FMC.classe = XZAEC_ClasseBouchon and 
				CFG..TYP_FMC.numero = EXP..FMC_GEN.type and 
				EXP..FMC_GEN.horodate_lien_cause <= @va_Horodate_in
	
			if @@rowcount > 0
				select @vl_TeteBouchon = XDC_OUI
		end
	
		if @vl_TeteBouchon = XDC_NON
		begin
			/*B cas d'evenement pas de type tete de bouchon */
			insert #LISTE 
			values (@va_NumEvt_in, @va_CleEvt_in )
		end
	
		/*A creation d'un curseur pour parcourir la liste */
		declare Pointeur cursor
		for select numero, cle
		from #LISTE
		open Pointeur
	
		/* parcours de la liste */
		fetch Pointeur into @vl_NumEvt, @vl_CleEvt
		while (@@sqlstatus = 0)
		begin
			/*A liste des consequences de l'evenement en enlevant */
			/*A les bouclages possibles                           */
			insert LISTE_CONSEQ ( numero , cle , type , numero_pere , cle_pere, spid )
			select
				EXP..FMC_GEN.numero , 
				EXP..FMC_GEN.cle, 
				EXP..FMC_GEN.type,
				@va_NumEvt_in,
				@va_CleEvt_in,
				@@spid
			from EXP..FMC_GEN
			where	EXP..FMC_GEN.cause =  @vl_NumEvt and 
				EXP..FMC_GEN.cle_cause = @vl_CleEvt and 
				EXP..FMC_GEN.horodate_lien_cause <= @va_Horodate_in and
				not exists ( select * from LISTE_CONSEQ, EXP..FMC_GEN
					     where  EXP..FMC_GEN.cause =  @vl_NumEvt and
						EXP..FMC_GEN.cle_cause = @vl_CleEvt and
						EXP..FMC_GEN.horodate_lien_cause <= @va_Horodate_in and
						LISTE_CONSEQ.numero = EXP..FMC_GEN.numero and
						LISTE_CONSEQ.cle = EXP..FMC_GEN.cle and
						LISTE_CONSEQ.spid = @@spid)
		
			/*B evenement suivant  de niveau 0 */
			fetch Pointeur into @vl_NumEvt, @vl_CleEvt
		end
	end
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/* Adaptation PIT pour attente remontee replication */
	select @vl_replication_ok=0
	select @vl_timeout=0
	while ( @vl_replication_ok = 0 and @vl_timeout < 50 )
	begin
		select @vl_timeout=@vl_timeout+1
		select @vl_NumEvt = numero from EXP..FMC_GEN
		where EXP..FMC_GEN.numero = @va_NumEvt_in and
			EXP..FMC_GEN.cle = @va_CleEvt_in

		if @@rowcount > 0
			select @vl_replication_ok = 1

		waitfor delay "00:00:00.100"
	end

	/*A recherche du type de l'evenement */
	select @vl_Classe = CFG..TYP_FMC.classe from EXP..FMC_GEN, CFG..TYP_FMC
	where	EXP..FMC_GEN.numero = @va_NumEvt_in and 
		EXP..FMC_GEN.cle = @va_CleEvt_in and
		CFG..TYP_FMC.numero = EXP..FMC_GEN.type

	/*A liste des consequences de l'evenement */
	/*! table non utilisee dans ce cas */
	if @@rowcount != 1
		return	XDC_NOK

	/*A liste temporaire des evenements de niveau 0 */
	create table #LISTE (numero int, cle tinyint)

	/*A Test si c'est une tete de bouchon */
	if @vl_Classe = XZAEC_ClasseGenerique 
	begin
		/*B recherche les queues de bouchon : la consequence queue */
		/*B de bouchon est sautee                                  */
		insert #LISTE( numero , cle )
		select  EXP..FMC_GEN.numero ,  EXP..FMC_GEN.cle 
		from EXP..FMC_GEN, CFG..TYP_FMC
		where	EXP..FMC_GEN.cause = @va_NumEvt_in and 
			EXP..FMC_GEN.cle_cause = @va_CleEvt_in  and
			CFG..TYP_FMC.classe = XZAEC_ClasseBouchon and 
			CFG..TYP_FMC.numero = EXP..FMC_GEN.type and 
			EXP..FMC_GEN.horodate_lien_cause <= @va_Horodate_in

		if @@rowcount > 0
			select @vl_TeteBouchon = XDC_OUI
	end

	if @vl_TeteBouchon = XDC_NON
	begin
		/*B cas d'evenement pas de type tete de bouchon */
		insert #LISTE 
		values (@va_NumEvt_in, @va_CleEvt_in )
	end

	/*A creation d'un curseur pour parcourir la liste */
	declare Pointeur cursor
	for select numero, cle
	from #LISTE
	open Pointeur

	/* parcours de la liste */
	fetch Pointeur into @vl_NumEvt, @vl_CleEvt
	while (@@sqlstatus = 0)
	begin
		/*A liste des consequences de l'evenement en enlevant */
		/*A les bouclages possibles                           */
		insert LISTE_CONSEQ ( numero , cle , type , numero_pere , cle_pere, spid )
		select
			EXP..FMC_GEN.numero , 
			EXP..FMC_GEN.cle, 
			EXP..FMC_GEN.type,
			@va_NumEvt_in,
			@va_CleEvt_in,
			@@spid
		from EXP..FMC_GEN
		where	EXP..FMC_GEN.cause =  @vl_NumEvt and 
			EXP..FMC_GEN.cle_cause = @vl_CleEvt and 
			EXP..FMC_GEN.horodate_lien_cause <= @va_Horodate_in and
			not exists ( select * from LISTE_CONSEQ, EXP..FMC_GEN
				     where  EXP..FMC_GEN.cause =  @vl_NumEvt and
					EXP..FMC_GEN.cle_cause = @vl_CleEvt and
					EXP..FMC_GEN.horodate_lien_cause <= @va_Horodate_in and
					LISTE_CONSEQ.numero = EXP..FMC_GEN.numero and
					LISTE_CONSEQ.cle = EXP..FMC_GEN.cle and
					LISTE_CONSEQ.spid = @@spid)
	
		/*B evenement suivant  de niveau 0 */
		fetch Pointeur into @vl_NumEvt, @vl_CleEvt
	end
#endif
#endif

	/*A appel direct via applix */
	if @va_Memorisation_in != XZAEC_MemoriserConseq
	begin
		/*B valeurs retournees par la procedure */
		select 
			numero,
			cle,
			type
		from LISTE_CONSEQ
		where spid = @@spid

		delete LISTE_CONSEQ where spid = @@spid
	end

	return XDC_OK
go
