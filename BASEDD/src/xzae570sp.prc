/*E*/
/* Fichier : $Id: xzae570sp.prc,v 1.2 2019/01/17 18:21:59 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2019/01/17 18:21:59 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae570sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG		13/05/18 : Creation GABEB (DEM/1283)
* JMG		28/08/18 : correction requete sens 1.2
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzae570sp.h" */

/* static char *version = "@(#)xzae570.c	1.1 23/18/94 : xzae570" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE570' and type = 'P')
	drop procedure XZAE570
go

	create procedure XZAE570
	   @va_Horodate_in      datetime = null,
	   @va_Autoroute_in		tinyint,
	   @va_Sens_in			tinyint,
	   @va_pr_in			int,
	   @va_ech_in			smallint
	   as 

/* 
*
* SP    XZAO553SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*  					va_Horodate_in
*  					va_Autoroute_in
* 					va_Sens_in
*  					va_Debut_in
*  					va_Fin_in
*
* PARAMETRES EN SORTIE :
*

* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* 
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Fev 2012    : Creation (DEM/1016)
------------------------------------------------------*/
declare @vl_Status int,
	@vl_delta int,
	@vl_numero int, @vl_cle tinyint,@vl_prQ int, @vl_prT int,
	@vl_valide int,    @vl_min int, @vl_max int
		
        /*A
        ** Verification que le serveur est bien celui du SDHIS
        */
 if @va_Horodate_in = null
	select @va_Horodate_in=getdate()

	if @va_Autoroute_in = null or @va_Sens_in = null
		or @va_pr_in=null 
			return XDC_ARG_INV

create table #LISTE_EVT(numero          int,
                                cle            tinyint ,
                                numeroT           int             null,
                                cleT       tinyint         null,
                                autorouteQ       tinyint null,
                                PRQ              int             null,
                                sensQ            tinyint         null,
				point_caracteristiqueQ tinyint               null    ,
				numero_point_caracteristiqueQ    tinyint               null,
                                autorouteT       tinyint null,
                                PRT              int             null,
                                sensT           tinyint         null,
				point_caracteristiqueT tinyint               null    ,
				numero_point_caracteristiqueT    tinyint               null,
				numero_cause int             null,
				cle_cause tinyint         null,
				valide int null,
				constraint PK_LISTE primary key (numero, cle))

	/*tous les bouchons en cours*/
	insert #LISTE_EVT
	select GEN.numero, GEN.cle, GEN.cause, GEN.cle_cause,
		HIS.autoroute, HIS.PR , HIS.sens,
		HIS.point_caracteristique, HIS.numero_point_caracteristique,
		HIS2.autoroute, HIS2.PR, HIS2.sens,
		HIS2.point_caracteristique, HIS2.numero_point_caracteristique,
		GEN2.cause, GEN2.cle_cause,
		0
	from   EXP..FMC_GEN GEN,
			EXP..FMC_GEN GEN2,
	   EXP..FMC_HIS_DER HIS,
		EXP..FMC_HIS_DER HIS2
	where  GEN.numero = HIS.numero
	and	   GEN.cle = HIS.cle
	and	   HIS.autoroute = @va_Autoroute_in
	and 	   GEN.cle_cause = HIS2.cle
	and	   GEN.cause = HIS2.numero
	and	   GEN.debut <=  @va_Horodate_in
	and	   GEN.fin = null
	and	   GEN.type in ( XZAEC_FMC_QueueBouchon)
	and		GEN.numero = HIS.numero
	and		GEN.cle = HIS.cle
	and		GEN2.numero = GEN.cause
	and		GEN2.cle = GEN.cle_cause

	/*on garde ceux dont tete ou queue est sur l echangeur en entree*/
	update #LISTE_EVT
	set valide = 1
	where (point_caracteristiqueQ=XDC_POINT_CARACT_ECHANGEUR and numero_point_caracteristiqueQ = @va_ech_in and sensQ = @va_Sens_in)
	or (point_caracteristiqueT =XDC_POINT_CARACT_ECHANGEUR and numero_point_caracteristiqueT = @va_ech_in and sensT = @va_Sens_in)

	/*on garde ceux dans le bon sens*/
	if @va_Sens_in = XDC_SENS_SORTIE_SUD
	begin
		update  #LISTE_EVT
		set valide = -1
		where sensQ != XDC_SENS_SUD and sensQ != XDC_SENS_SORTIE_SUD
			and sensT != XDC_SENS_SUD and sensT != XDC_SENS_SORTIE_SUD
		and valide = 0
	end
	else
	begin
		update  #LISTE_EVT
		set valide = -1
		where sensQ != XDC_SENS_NORD and sensQ != XDC_SENS_SORTIE_NORD
			and sensT != XDC_SENS_NORD and sensT != XDC_SENS_SORTIE_NORD
		and valide = 0
	end

	/*on garde ceux dont le PR est proche*/
	declare ptr cursor
	for select numero, cle, PRQ, PRT
	from #LISTE_EVT
	where valide = 0
	for update of valide

	open ptr
	fetch ptr into @vl_numero, @vl_cle, @vl_prQ, @vl_prT
	select @vl_delta = 100
	while (@@sqlstatus = 0)
	begin
		select @vl_valide = -1
		if @vl_prQ < @vl_prT
		begin
			select @vl_min = @vl_prQ
			select @vl_max = @vl_prT
		end
		else
		begin
			select @vl_min = @vl_prT
			select @vl_max =  @vl_prQ
		end

			/*bouchons qui englobent la sortie*/
			if @vl_min<=@va_pr_in and @vl_max>=@va_pr_in 
			begin
				select @vl_valide = 1
			end
			/*sens 1 bouchon finit avant la sortie mais tete pas trop loin*/
			/*sens 2 bouchon commence avant la sortie mais queue pas trop loin*/
			else if @vl_max < @va_pr_in and @vl_max > @va_pr_in - @vl_delta
			begin
				select @vl_valide = 1
			end
			/* sens 1 :bouchon commence apres mais queue juste apres la sortie*/
			/* sens 2 :bouchon finit apres mais tete juste apres la sortie*/
			else if @vl_min > @va_pr_in and @vl_min - @vl_delta <= @va_pr_in
			begin
				select @vl_valide = 1
			end
			update #LISTE_EVT
			set valide =@vl_valide
			where numero = @vl_numero and cle = @vl_cle
		fetch ptr into @vl_numero, @vl_cle, @vl_prQ, @vl_prT
	end
	close ptr	

	select numero, cle, numeroT, cleT,
			autorouteQ, sensQ, PRQ, 
			PRT,
			numero_point_caracteristiqueQ, numero_point_caracteristiqueT,
			numero_cause, cle_cause,
			autorouteT, sensT
	from #LISTE_EVT
	where valide=1
	return XDC_OK
go  
