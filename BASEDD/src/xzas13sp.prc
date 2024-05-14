/*E*/
/*  Fichier : $Id: xzas13sp.prc,v 1.6 2013/09/02 11:25:35 gesconf Exp $      Release : $Revision: 1.6 $        Date : $Date: 2013/09/02 11:25:35 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas13sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	03/02/97	: Creation	(V 1.1)
* JMG	18/04/12 	: tdp mz avec historique v1.2
* JMG	09/10/12	: correction recup donnees si historique 1.4
* PNI	02/09/13	: correction pour date dans l'historique 1.5
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare une courbe pour les mesures TDP 6 minutes
* sur une periode de 24 heures si possible
* 
* Sequence d'appel
* SP	XZAS13_Lecture_ZDP_6mn
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

drop procedure XZAS13
go


create procedure XZAS13
	@va_Zdp_in		smallint	= null,
	@va_HorodateDebut_in	datetime	= null
as
	declare @vl_Valeur smallint, @vl_Validite tinyint,
		@vl_DerniereDate datetime,@vl_Horodate datetime, @vl_NbMesures int

	/*A controle parametres en entree */
	if (@va_Zdp_in = null or
	   @va_HorodateDebut_in = null )
		return XDC_ARG_INV
#ifdef HIST
	/* creation d'une table de stockage de la courbe */
	create table #CURV_MES ( numero smallint null ,valeur smallint null )
	
	select @vl_DerniereDate=getdate()

	if (datediff (day, @va_HorodateDebut_in, @vl_DerniereDate) > 0)
	begin
		/*mode historique*/
		select @vl_DerniereDate = dateadd (hour,24,@va_HorodateDebut_in)
	end

	/*B Lecture des mesures des 6 dernieres heures */
	select @vl_NbMesures = 0
	while dateadd(minute, @vl_NbMesures*6,@va_HorodateDebut_in) < @vl_DerniereDate
	begin
		/* lecture d'une mesure a une date precisee et de type demande */
		select
			@vl_Valeur 	= temps,
			@vl_Validite 	= validite,
			@vl_Horodate 	= horodate
		from HIS..ZDP_6MN
	        where   numero = @va_Zdp_in and
			horodate = dateadd(minute, @vl_NbMesures*6, 
							   @va_HorodateDebut_in)
	
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
#else
	/* creation d'une table de stockage de la courbe */
	create table #CURV_MES ( numero smallint null ,valeur smallint null )
	
	select @vl_DerniereDate=getdate()

	if (datediff (day, @va_HorodateDebut_in, @vl_DerniereDate) > 0)
	begin
		/*mode historique*/
		select @vl_DerniereDate = dateadd (hour,24,@va_HorodateDebut_in)
		exec SQL_HIS.PRC..XZAS13 @va_Zdp_in,@va_HorodateDebut_in
	end
	else
	begin
	/*B Lecture des mesures des 6 dernieres heures */
	select @vl_NbMesures = 0
	while dateadd(minute, @vl_NbMesures*6,@va_HorodateDebut_in) < @vl_DerniereDate
	begin
		/* lecture d'une mesure a une date precisee et de type demande */
		select
			@vl_Valeur 	= temps,
			@vl_Validite 	= validite,
			@vl_Horodate 	= horodate
		from EXP..ZDP_6MN
	        where   numero = @va_Zdp_in and
			horodate = dateadd(minute, @vl_NbMesures*6, 
							   @va_HorodateDebut_in)
	
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
	end	
	return XDC_OK
#endif
go
