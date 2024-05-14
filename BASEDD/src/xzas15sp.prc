/*E*/
/*  Fichier : $Id: xzas15sp.prc,v 1.3 2004/07/16 11:42:56 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2004/07/16 11:42:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas15sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	03/02/97	: Creation	(V 1.1)
* P.C   27/07/00        : requete sous HIS et non plus sous EXP (1.3)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare une courbe pour les mesures TDP 6 minutes
* sur une periode de 24 heures si possible
* 
* Sequence d'appel
* SP	XZAS15_Lecture_ZDP_6mn_Applix
* 
* Arguments en entree
* XDY_Mot	va_zdp_in		: numero de zone de temps de parcours
* XDY_Datetime	va_HorodateDebut_in	: date de debut des mesures
* 
* Arguments en sortie
* 
** 
* Code retour
* XDC_OK
* XDC_NOK		: probleme d'acces aux mesures
* XDC_ARG_INV		: parametres d'entree null, ou valeur non autorisee ou
*                 	  zdp inexistante
* 
* Conditions d'utilisation
* l'horodate fournie doit etre une 6 minute
*
* Fonction
* selectionner dans la table MESURE_TDP_6_6MNUTES
-------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAS15' and type = 'P')
	drop procedure XZAS15
go

create procedure XZAS15
	@va_Zdp_in		smallint	= null,
	@va_HorodateDebut_in	datetime	= null,
	@va_debut_out		datetime 	= null output
as
	declare @vl_Valeur smallint, @vl_Validite tinyint,
		@vl_DerniereDate datetime,@vl_Horodate datetime, @vl_NbMesures int

	/*A controle parametres en entree */
	if (@va_Zdp_in = null or
	   @va_HorodateDebut_in = null )
		return XDC_ARG_INV

	/* creation d'une table de stockage de la courbe */
	create table #CURV_MES ( numero smallint null ,valeur smallint null )
	
	/*recherche de l'horodate de debut*/
	select @va_debut_out=horodate
	from HIS..ZDP_6MN
	where numero = @va_Zdp_in and
	horodate >= @va_HorodateDebut_in and
	horodate < dateadd(minute, 6, @va_HorodateDebut_in)

	if @@rowcount = 0
		select @va_debut_out = @va_HorodateDebut_in

	/*heure de fin*/
	select @vl_DerniereDate=dateadd(hour, 24, @va_debut_out)

	/*B Lecture des mesures des 6 dernieres heures */
	select @vl_NbMesures = 1
	while dateadd(minute, @vl_NbMesures*6,@va_debut_out) < @vl_DerniereDate
	begin
		/* lecture d'une mesure a une date precisee et de type demande */
		select
			@vl_Valeur 	= temps,
			@vl_Validite 	= validite,
			@vl_Horodate 	= horodate
		from HIS..ZDP_6MN
	        where   numero = @va_Zdp_in and
			horodate >= dateadd(minute, (@vl_NbMesures-1)*6,
						   @va_debut_out)
			and horodate <dateadd(minute, @vl_NbMesures*6, 
							   @va_debut_out)
	
		/* memoriser la valeur de la mesure                           */
		/* si pas de valeur : ajouter une valeur 0, validite inconnue */
		if  @@rowcount = 0 or @vl_Validite = XDC_FAUX
			insert #CURV_MES ( numero, valeur)
			values ( @vl_NbMesures , 0 )
		else	insert #CURV_MES (numero, valeur)
			values (@vl_NbMesures , @vl_Valeur)
	
		/* numero de la mesure lue */
		select @vl_NbMesures = @vl_NbMesures + 1
	
	end

	/*A Retourner la courbe */
	select valeur from #CURV_MES
	order by numero
	
	return XDC_OK
go
