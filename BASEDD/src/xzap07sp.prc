/*E*/
/*Fichier : $Id: xzap07sp.prc,v 1.8 2018/10/19 17:41:18 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2018/10/19 17:41:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap07sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Sep 1994	: Creation
* gaborit	21 Sep 1994	: Rien (v1.2)
* gaborit	21 Sep 1994	: Rien (v1.3)
* volcic	13 Oct 1994	: Modification (v1.4)
* volcic	28 Oct 1994	: Modification reception chaine nulle (v1.5)
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.6
* PNI		04 Jul 2013 	: v�rif l'existance d'un destinataire fax: evite les proposition sans interlocuteur ou destinataire v1.7 FE114
* LCL		07 Sep 2018	: ajout sms LOT25 DEM1301 1.8
* GGY	26/09/23 : Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une proposition d'envoi FAX a un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

drop procedure XZAP07
go

create procedure XZAP07

	@NumPA_In		smallint,
	@Ordre_In		smallint,
	@District_In		tinyint,
	@Remarque_In		char(50),
	@Explication_In		char(250),
	@Libelle_In		char(25),
	@Sms_in 		tinyint = null
	

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
* XDY_Mot 		Ordre_In
* XDY_District		District_In
* XDY_Phrase		Remarque_In
* XDY_Commentaire	Explication_In
* XDY_Nom		Libelle_In
* XDY_Octet		Sms_in
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
* FONCTION :
*
* Ajoute dans la table PA_FAX
*
* MECANISMES :
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzap07sp.h" */

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* static char *version = "@(#)xzap07sp.prc	1.1 08/25/94 : xzap07sp" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */

as 
declare @vl_nb           int    

#ifdef HIST
declare @vl_Status int, @vl_Site tinyint

	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAP07
		@NumPA_In		,
		@Ordre_In		,
		@District_In		,
		@Remarque_In		,
		@Explication_In		,
		@Libelle_In	,
		@Sms_in

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP07
		@NumPA_In		,
		@Ordre_In		,
		@District_In		,
		@Remarque_In		,
		@Explication_In		,
		@Libelle_In	,
		@Sms_in

        if @vl_Status = null
                return XDC_PRC_INC
        else return @vl_Status
#else
   /*A 
   ** Test des parametres d'entree :
   **    Si un des parametres d'entree est egal a null alors XDC_NOK
   */

   if      @NumPA_In = null 
   	or @Ordre_In = null 
   	or @District_In = null
      
      return XDC_NOK
   else   
      begin
   
      /*A
      ** Ajout dans la table SAD_FAX (PA_FAX) d'une proposition d'envoi
      ** Fax a un plan d'actions
      */
   
      if @Remarque_In = null
         select @Remarque_In = ' '
      
      if @Explication_In = null
         select @Explication_In = ' '
      
      if @Libelle_In = null
         select @Libelle_In = ' '

      /* FE 114, n'ins�re la proposition que s'il existe une comp�tence geographique: evite les proposition sans interlocuteur ou destinataire */
      select @vl_nb=count(*) from EXP..SAD_DST_FAX where numero=@NumPA_In

      if @vl_nb > 0
      begin
      
      
      insert into EXP..SAD_FAX (numero, ordre, district, remarque, explication, libelle, sms)

      values ( @NumPA_In, @Ordre_In, @District_In, @Remarque_In, @Explication_In, @Libelle_In, @Sms_in )
      end
      return XDC_OK
      
      end
#endif
go
