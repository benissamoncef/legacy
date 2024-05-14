/*E*/
/*  Fichier : $Id: xzas20sp.prc,v 1.4 1995/11/08 16:06:06 gesconf Exp $     Release : $Revision: 1.4 $     Date : $Date: 1995/11/08 16:06:06 $ 
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas20sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Analyse 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.F.  29/11/94        : Creation      (V 1.1)
* B.G.	24/10/95	: gestion du sens inverse A57 (1.2)
* B.G.	06/11/95	: optimisation (vitesse) (1.3)
* B.G.  08/11/95	: modif bug @@rowcount (1.4)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare les donnees qui serviront a tracer les courbes de retard
* sur itineraire
*
* Sequence d'appel
* SP    XZAS20_Calcul_Retard_Itineraire
*
* Arguments en entree :
* XDY_Mot       va_NumEchangDeb_in      : numero du premier echangeur 
* XDY_Mot       va_NumEchangFin_in      : numero du deuxieme echangeur
* XDY_Datetime  va_Horodate_in		: date du calcul (Heure H) 
* XDY_Mot       va_A0_in...va_A1O_in    : Coefficients parametrables
*					  (estimation H+1)
*
* Arguments en sortie :
*
* Liste retournee :
* XDY_Nom	nom		: nom de l'echangeur
* XDY_PR	PR		: PR de l'echangeur
* XDY_Entier	retard_H	: retard (Heure H)
* XDY_Entier    retard_H_M1     : retard (Heure H - 1)
* XDY_Entier    retard_H_P1     : retard (Heure H + 1)
* XDY_Entier	cumul_H		: cumul du temps (Heure H)
* XDY_Entier    cumul_H_M1      : cumul du temps (Heure H - 1)
* XDY_Entier    cumul_H_P1      : cumul du temps (Heure H + 1)
*
* Code retour
* XDC_OK
* XDC_NOK               : probleme d'acces aux donnees 
* XDC_ARG_INV           : parametres d'entree null, ou valeur non autorisee ou
*                         echangeur inexistante
*
* Conditions d'utilisation
* Les deux echangeurs sont localises sur le meme autoroute.
*
* Fonction
*
-------------------------------------------------------*/

use PRC
go

create procedure XZAS;20
	@va_NumEchangDeb_in       tinyint        = null,
	@va_NumEchangFin_in       tinyint        = null,
	@va_Horodate_in    	  datetime       = null,
	@va_A0_in                 int            = null,
	@va_A1_in                 int            = null,
	@va_A2_in                 int            = null,
	@va_A3_in                 int            = null,
	@va_A4_in                 int            = null,
	@va_A5_in                 int            = null,
	@va_A6_in                 int            = null,
	@va_A7_in                 int            = null,
	@va_A8_in                 int            = null,
	@va_A9_in                 int            = null,
	@va_A10_in                int            = null
as
	declare @vl_PRDeb int,			/* PR debut	*/ 
		@vl_PRFin int,			/* PR fin 	*/
		@vl_PR    int,			/* PR intermediaire */
		@vl_Retard_H   int,             /* Retard de parcours H */
		@vl_Retard_HM1 int,             /* Retard de parcours H-1 */
		@vl_Retard_HP1 int,             /* Retard de parcours H+1 */
		@vl_Cumul_H int,		/* Cumul a l'instant H */
		@vl_Cumul_HM1 int,              /* Cumul a l'instant H-1 */
		@vl_Cumul_HP1 int,              /* Cumul a l'instant H+1 */
		@vl_IndiceEchg smallint,	/* Induce de reperage */
		@vl_NbEnrg     smallint,	/* Nombre d'echangeurs */
		@vl_NumPortionDeb smallint,	/* Numero de portion debut */
		@vl_NumPortionFin smallint,	/* Numero de portion fin */
		@vl_Sens 	 tinyint,	/* Sens de deplacement */
		@vl_AutorouteDeb tinyint,	/* Autoroute */
		@vl_AutorouteFin tinyint,	/* Autoroute */
		@vl_CodeRetour tinyint,         /* Code de retour */
		@vl_SomCoeff  float             /* Somme des coefficients */

	/* Test Argument APPLIX (il n'existe pas d'equipement de type zero) */
	if @va_NumEchangDeb_in = 0 select @va_NumEchangDeb_in = null
	if @va_NumEchangFin_in = 0 select @va_NumEchangFin_in = null

	/*A Controle des parametres en entree */
	if @va_NumEchangDeb_in = null or
	   @va_NumEchangFin_in = null or
	   @va_Horodate_in = null
		return XDC_ARG_INV

	/* Initialisation des coef nuls a zero */
	if @va_A0_in is null    select @va_A0_in = 0
	if @va_A1_in is null    select @va_A1_in = 0
	if @va_A2_in is null    select @va_A2_in = 0
	if @va_A3_in is null    select @va_A3_in = 0
	if @va_A4_in is null    select @va_A4_in = 0
	if @va_A5_in is null    select @va_A5_in = 0
	if @va_A6_in is null    select @va_A6_in = 0
	if @va_A7_in is null    select @va_A7_in = 0
	if @va_A8_in is null    select @va_A8_in = 0
	if @va_A9_in is null    select @va_A9_in = 0
	if @va_A10_in is null   select @va_A10_in = 0

	/* Calcul de la somme des coefficients */
	select @vl_SomCoeff = (@va_A0_in+@va_A1_in+@va_A2_in+@va_A3_in +
	      @va_A4_in+@va_A5_in+@va_A6_in+@va_A7_in+@va_A8_in +
	      @va_A9_in+@va_A10_in)

        /* Test validite des coefficients */
	if @vl_SomCoeff = 0 return XDC_ARG_INV

	create table #COEFF(
		indice  int,
		coeff   float
		)

	/* Conversion et mise a jour de la table temporaire */
	insert into #COEFF (indice,coeff)
	values (0,(@va_A0_in/@vl_SomCoeff))
	insert into #COEFF (indice,coeff)
	values (1,(@va_A1_in/@vl_SomCoeff))
	insert into #COEFF (indice,coeff)
	values (2,(@va_A2_in/@vl_SomCoeff))
	insert into #COEFF (indice,coeff)
	values (3,(@va_A3_in/@vl_SomCoeff))
	insert into #COEFF (indice,coeff)
	values (4,(@va_A4_in/@vl_SomCoeff))
	insert into #COEFF (indice,coeff)
	values (5,(@va_A5_in/@vl_SomCoeff))
	insert into #COEFF (indice,coeff)
	values (6,(@va_A6_in/@vl_SomCoeff))
	insert into #COEFF (indice,coeff)
	values (7,(@va_A7_in/@vl_SomCoeff))
	insert into #COEFF (indice,coeff)
	values (8,(@va_A8_in/@vl_SomCoeff))
	insert into #COEFF (indice,coeff)
	values (9,(@va_A9_in/@vl_SomCoeff))
	insert into #COEFF (indice,coeff)
	values (10,(@va_A10_in/@vl_SomCoeff))

	/*A verifier l'existence des echangeurs */

	/* Recherche du PR du 1er echangeur (PR de debut) */
	select @vl_PRDeb = PR, @vl_NumPortionDeb = portion
	from CFG..RES_ECH
	where numero = @va_NumEchangDeb_in

	if @@rowcount != 1
		return XDC_ARG_INV

	/* Recherche du PR du 2eme echangeur (PR de fin) */
	select @vl_PRFin = PR, @vl_NumPortionFin = portion
	from CFG..RES_ECH
	where numero = @va_NumEchangFin_in

	if @@rowcount != 1
		return XDC_ARG_INV

	/* Test de coherence : les deux echangeurs doivent etre sur  	*/
	/* la meme autoroute.						*/
	select @vl_AutorouteDeb = autoroute
	from CFG..RES_POR
	where numero = @vl_NumPortionDeb

	if @@rowcount != 1
		return XDC_ARG_INV

	select @vl_AutorouteFin = autoroute
	from CFG..RES_POR
	where numero = @vl_NumPortionFin

	if @@rowcount != 1
		return XDC_ARG_INV

	/* Les autoroutes doivent etre identiques */
	if @vl_AutorouteDeb != @vl_AutorouteFin
		return XDC_ARG_INV

	/*A recherche du sens de deplacement.			    */
	/* Test sens : sens = 1 (XDC_SENS_SUD) si PR  croissants    */
	/*	       sens = 2 (XDC_SENS_NORD) si PR  decroissants */
	if (@vl_PRFin > @vl_PRDeb)
		select @vl_Sens = XDC_SENS_SUD 
	else
	begin
		/* Inversion des PR pour avoir PRFin > PRDeb */
		select @vl_PR = @vl_PRFin
		select @vl_PRFin = @vl_PRDeb
		select @vl_PRDeb = @vl_PR
		select @vl_Sens = XDC_SENS_NORD 
	end
	
	/* Creation d'une table temporaire utilisee pour le stockage 	*/
	/* des informations que seront retournees comme resultat:    	*/
	/* 	- informations concernant les echangeurs             	*/
	/*	- les donnees calculees (retard et cumul) 		*/
	create table #CURV_RET (nom		char(40),
				numero          tinyint,
				PR		int,
				retard_H	int null,
				retard_HM1      int null,
				retard_HP1      int null,
				cumul_H		int null,
				cumul_HM1       int null,
				cumul_HP1       int null,
				indice		smallint null)


	/* Recherche de la liste des echangeurs de l'autoroute */
	/* compris entre les PR de debut et de fin.            */
	if @vl_Sens = XDC_SENS_SUD
	   insert #CURV_RET (	numero,
				PR,
				nom)
		select  CFG..RES_ECH.numero, 
			CFG..RES_ECH.PR, 
			CFG..RES_ECH.nom 
		from CFG..RES_ECH, CFG..RES_POR
		where CFG..RES_ECH.PR >= @vl_PRDeb
		and   CFG..RES_ECH.PR <= @vl_PRFin
		and   CFG..RES_ECH.portion = CFG..RES_POR.numero 
		and   CFG..RES_POR.autoroute = @vl_AutorouteDeb
		order by CFG..RES_ECH.PR
	else
	   insert #CURV_RET (	numero,
				PR,
				nom)
		select  CFG..RES_ECH.numero, 
			CFG..RES_ECH.PR, 
			CFG..RES_ECH.nom 
		from CFG..RES_ECH, CFG..RES_POR
		where CFG..RES_ECH.PR >= @vl_PRDeb
		and   CFG..RES_ECH.PR <= @vl_PRFin
		and   CFG..RES_ECH.portion = CFG..RES_POR.numero 
		and   CFG..RES_POR.autoroute = @vl_AutorouteDeb
		order by CFG..RES_ECH.PR desc

	/* Nombre d'enchangeur trouve */
	select @vl_NbEnrg = @@rowcount

	/*A traitement de l'A57 */
	
	if exists (	select * from CFG..RES_AUT_INV
			where	numero=@vl_AutorouteDeb
			  and	inversee=XDC_PR_INV )
	begin
		if @vl_Sens = XDC_SENS_NORD
			select @vl_Sens = XDC_SENS_SUD
		else
			select @vl_Sens = XDC_SENS_NORD
	end

	/* Indice de reperage des echangeurs :                  */
	/*      indice = 1 represente le premier echangeur      */
	select @vl_IndiceEchg = 1

	/* On ne travail que sur le premier enregistrement satisfaisant	*/
	/* la condition (Prise en compte dans l'ordre des PR croissants */
	set rowcount 1

	/* Parcours des enregistrements (un par un) et mise a jour de	*/
	/* l'indice de facon a faciliter la recherche de deux echangeurs*/
	/* consecutifs							*/
	while (@vl_IndiceEchg <= @vl_NbEnrg)
	begin
		update #CURV_RET
		set indice = @vl_IndiceEchg
		where indice = null

		select @vl_IndiceEchg = @vl_IndiceEchg + 1
	end

	/* Retour au mode normal */
	set rowcount 0

	/* Calcul des informations intermediaires pour chaque couple	*/
	/* consecutif d'echangeur.					*/
	/* Parcours des echangeurs avec association par ordre de PR	*/
	/* croissant.							*/
	/* Il y a : (nombre echangeurs - 1) couples consecutifs		*/
	/* Preparation du resultat final (synthese):                        */
	/* Au niveau du premier echangeur, le retard et le cumul sont       */
	/* toujours nuls.                                                   */
	/* Lorsqu'un retard est negatif, c'est qu'il n'a pas pu etre        */
	/* calcule (Zone d'influence inexitante, pas de station de comptage */
	/* ou donnees insuffisante). Dans ce cas, le cumul reste constant.  */

	/* Positionnement au niveau du premier echangeur */
	select @vl_IndiceEchg = 1

	/* Il n'y ni retard ni cumul pour le premier echangeur */
	update #CURV_RET
	set     retard_H    = 0,        
		retard_HM1  = 0,
		retard_HP1  = 0,
		cumul_H     = 0,
		cumul_HM1   = 0,
		cumul_HP1   = 0
	where indice = @vl_IndiceEchg

	while (@vl_IndiceEchg < @vl_NbEnrg)
	begin
		/* Premier echangeur du couple */
		select 	@vl_PRDeb = PR
		from #CURV_RET
		where indice = @vl_IndiceEchg

		/* Deuxieme echangeur du couple */
		select 	@vl_PRFin = PR 
		from #CURV_RET
		where indice = @vl_IndiceEchg + 1

		if (@vl_PRDeb > @vl_PRFin)
		begin
			/* Inversion des PR pour avoir PRFin > PRDeb */
			select @vl_PR = @vl_PRFin
			select @vl_PRFin = @vl_PRDeb
			select @vl_PRDeb = @vl_PR
		end

		/*A Calcul du retard pour le couple d'echangeur. */
		/* Parametres transmis:				 */
		/* @vl_PRDeb: PR du premier echangeur 			    */
		/* @vl_PRFin: PR du second echangeur                        */
		/* @vl_AutorouteDeb: l'autoroute			    */
		/* @va_Horodate_in: date de reference			    */
		/* @vl_Sens: sens de parcours				    */
		/* Retourne le retard de parcours entre les deux echangeurs */
		/* (vl_Retard_H, vl_Retard_HM1 et vl_Retard_HP1)	    */
		/* - si le resultat = 0 alors il n'a pas de retard	    */
		/* - si le resultat > O alors il y a eu du retard	    */
		/* - si le resultat < O alors le retard n'est pas calculable*/ 
		/* Code de retour : vl_CodeRetour			    */
		/* XDC_OK						    */
		/* XDC_NOK               : probleme d'acces aux donnees     */
		/* XDC_ARG_INV           : parametres d'entree null, 	    */
		/*				ou valeur non autorisee ou  */
		/*                         	echangeur inexistante       */

		execute @vl_CodeRetour = PRC..XZAS;21 @vl_PRDeb,
						@vl_PRFin,
						@vl_AutorouteDeb,
						@va_Horodate_in,
						@vl_Sens,
						@vl_Retard_H   out,
						@vl_Retard_HM1 out,
						@vl_Retard_HP1 out

		/* Test du code de retour. */
		if @vl_CodeRetour != XDC_OK
		begin
			/* Retards non calculee pour cette zone */
			select @vl_Retard_H = -1
			select @vl_Retard_HM1 = -1
			select @vl_Retard_HP1 = -1
		end

		/* Test de coherence */
		/* Pour un retard negatif, le cumul est maintenu */
		/* a sa valeur precedente */ 
		if (@vl_Retard_H < 0) 
			select @vl_Cumul_H = 
				(select cumul_H
				 from #CURV_RET
				 where indice = @vl_IndiceEchg)
		else
			select @vl_Cumul_H = (@vl_Retard_H + 
				(select cumul_H
				 from #CURV_RET
				 where indice = @vl_IndiceEchg))

		if (@vl_Retard_HM1 < 0) 
			select @vl_Cumul_HM1 = 
				(select cumul_HM1
				 from #CURV_RET
				 where indice = @vl_IndiceEchg)
		else                                     
		  	select @vl_Cumul_HM1 = (@vl_Retard_HM1 + 
		  		(select cumul_HM1
		  		 from #CURV_RET
	   			 where indice = @vl_IndiceEchg))

		if (@vl_Retard_HP1 < 0)
			select @vl_Cumul_HP1 =
				(select cumul_HP1
				 from #CURV_RET
				 where indice = @vl_IndiceEchg)
		else
			select @vl_Cumul_HP1 = (@vl_Retard_HP1 +
				(select cumul_HP1
				 from #CURV_RET
				 where indice = @vl_IndiceEchg))

		/* Mise a jour du retard et du cumul */
		update #CURV_RET
		set	retard_H =	@vl_Retard_H,
			retard_HM1 =	@vl_Retard_HM1,
			retard_HP1 =	@vl_Retard_HP1,
			cumul_H	= 	@vl_Cumul_H,
			cumul_HM1 =     @vl_Cumul_HM1,
			cumul_HP1 =     @vl_Cumul_HP1
		where indice = @vl_IndiceEchg + 1

		/* Passage au couple d'echangeurs  suivant */
		select @vl_IndiceEchg = @vl_IndiceEchg + 1
	end
	/* Fin parcours des echangeurs */

	/* Envoi du resultat final */
	select 	nom,		/* Nom de l'echangeur */
		PR,		/* PR de l'echangeur */
		retard_H,	/* Retard a l'heure H */
		retard_HM1,	/* Retard a l'heure H-1 */
		retard_HP1,	/* Retard a l'heure H+1 */
		cumul_H,	/* Cumul a l'heure H */
		cumul_HM1,	/* Cumul a l'heure H-1 */
		cumul_HP1	/* Cumul a l'heure H+1 */
	from #CURV_RET

	/* Destruction de la table temporaire */
	drop table #CURV_RET
	drop table #COEFF

 	return XDC_OK
go

