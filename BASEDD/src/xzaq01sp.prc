/*E*/
/*  Fichier : $Id: xzaq01sp.prc,v 1.4 1995/01/28 02:57:37 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/01/28 02:57:37 $ 
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaq01.sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* BASEDD 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.F.  22/12/94        : Creation      (V 1.1)
* B.G.  26/01/95        : modif      (V 1.4)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Mise a jour de l'etat d'une tache pour une machine d'un site
* L'ensemble des donnees est utilise par la suite pour le calcul
* de l'indice qualite.
*
* Sequence d'appel
* SP    XZAQ01_Mise_A_Jour_Etat
*
* Arguments en entree :
* XDY_Mot	@va_NomSite_in		: Nom du site
* XDY_Mot       @va_NomMachine_in       : Nom de la machine
* XDY_Mot       @va_NomTache_in         : Nom de la tache 
* XDY_Octet	@va_Etat_in		: Etat de la tache
*
* Arguments en sortie :
*
* Liste retournee :
*
* Code retour
* XDC_OK
* XDC_NOK               : probleme d'acces aux donnees 
* XDC_ARG_INV           : parametres d'entree null, ou valeur non autorisee ou
*                         echangeur inexistante
*
* Conditions d'utilisation
* Appele par la tache SUPERV lors du changement d'etat d'une tache.
*
* Fonction
*
-------------------------------------------------------*/

use PRC
go

create procedure XZAQ;01
	@va_NomSite_in		T_NOM		= null,
	@va_NomMachine_in       T_NOM           = null,
	@va_NomTache_in         T_NOM           = null,
	@va_Etat_in             bit		= 0
as
	declare	@vl_Date_Courante  datetime, /* Date courante */
		@vl_DerniereDate   datetime, /* Derniere date de misea jour */
		@vl_TotalOK	   int,	     /* Temps total tache OK */
		@vl_TotalNOK       int,      /* Temps total tache NOK */
		@vl_DecompteOK     int,      /* Nouveau decompte tache OK */
		@vl_DecompteNOK    int,      /* Nouveau decompte tache NOK */
		@vl_Decompte	   int,      /* Nouveau decompte de tempps */
		@vl_DerniereEtat   bit,	     /* Dernier etat de la tache */
		@vl_NomSite_in	   varchar(2),
		@vl_NomMachine_in  varchar(7),
		@vl_NomTache_in    varchar(6)


	/*A 
	** Controle des parametres en entree :
	*/
	if (@va_NomSite_in       = null) or
	   (@va_NomMachine_in    = null) or
	   (@va_NomTache_in      = null)
		return XDC_ARG_INV

	/* Test complementaire sur les valeurs attendues.*/
	if ((@va_Etat_in != XDC_OK) and (@va_Etat_in != XDC_NOK))
		return XDC_ARG_INV

	/*A cast des arguments */
	select  @vl_NomSite_in=@va_NomSite_in,
		@vl_NomMachine_in=@va_NomMachine_in,
		@vl_NomTache_in=@va_NomTache_in
	
	/*A
	** Mise ajour de la date courante.
	*/
	select @vl_Date_Courante = getdate()

	/*A
	** Recherche des informations concernant la tache.
	*/
	/*B
	** Il y un enregistrement unique pour une tache d'une machine
	** d'un site.
	*/
	select 	@vl_DerniereDate = DerniereDate,
		@vl_TotalOK = totalOK,
		@vl_TotalNOK =  totalNOK,
		@vl_DerniereEtat = DernierEtat
	from HIS..SYS_INQ
	where	site    =	@vl_NomSite_in
	and	machine =	@vl_NomMachine_in
	and	tache   =	@vl_NomTache_in

	/*A
	** Test permettant de determiner si la tache est deja
	** connues dans la Base de Donnees.
	*/
	if @@rowcount = 0
	begin
		/*B
		** Si la tache n'existe pas, il faut la creer.
		*/
		insert into HIS..SYS_INQ
		values (@va_NomSite_in,
			@va_NomMachine_in,
			@va_NomTache_in,
			0,
			0,
			@va_Etat_in,
			@vl_Date_Courante)
	end
	else
	begin
		/*B
		** Si la tache existe deja, il faut modifier sont etat et 
		** les totaux de disponibilite ou d'indisponibilite.
		*/

		/* Calcul du decompte de temps entre la derniere */
		/* mise a jour et l'heure actuelle.	         */
		select @vl_Decompte = abs( datediff(minute,
						@vl_Date_Courante,
						@vl_DerniereDate))


		select @vl_DecompteOK = 0
		select @vl_DecompteNOK = 0

		/*B
		** Comparaison entre le nouvel etat et celui enregistre.
		*/
		if @vl_DerniereEtat = @va_Etat_in
		begin
			/*B
			** Si l'etat n'a pas change par rapport au dernier
			** enregistrement.
			*/
			if @va_Etat_in = XDC_OK
			begin
				/* La tache reste en fonctionnement */
				select @vl_DecompteOK = @vl_TotalOK +
							@vl_Decompte
				select @vl_DecompteNOK = @vl_TotalNOK 
			end
			else
			begin
				/* La tache reste arretee */
				select @vl_DecompteOK = @vl_TotalOK
				select @vl_DecompteNOK = @vl_TotalNOK +
							 @vl_Decompte
			end
		end
		else
		begin
			/*B
			** Si l'etat a change par rapport au dernier 
			** enregistrement.
			*/
			if @va_Etat_in = XDC_OK
			begin
				/* La tache est relancee */
				select @vl_DecompteOK = @vl_TotalOK
				select @vl_DecompteNOK = @vl_TotalNOK  +
							 @vl_Decompte
			end
			else
			begin
				/* La tache est arrete */
				select @vl_DecompteOK = @vl_TotalOK + 
							@vl_Decompte
				select @vl_DecompteNOK = @vl_TotalNOK 
			end
		end

		/*B
		** Mise a jour des informations de la tache.
		*/
		update HIS..SYS_INQ
		set 	totalOK =  	@vl_DecompteOK,
			totalNOK= 	@vl_DecompteNOK,
			DernierEtat = 	@va_Etat_in,
			DerniereDate = 	@vl_Date_Courante
		where   site    =       @vl_NomSite_in
		and     machine =       @vl_NomMachine_in
		and     tache   =       @vl_NomTache_in
	end


	/*A
	** Fin de la procedure
	*/
 	return XDC_OK
go
