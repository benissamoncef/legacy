/*E*/
/*Fichier : $Id: xzap23sp.prc,v 1.1 1999/02/26 11:24:34 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/02/26 11:24:34 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap23sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	25 Nov 1998	: Creation
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une proposition d'export intergestionnaire a un plan d'actions
*  (C.I uniquement)
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP;23

	@NumPA_In		smallint,
	@Ordre_In		smallint,
	@District_In		tinyint,
	@Libelle_In		char(25),
	@Remarque_In		char(50),
	@Gravite_in		tinyint,
	@Trafic_in			tinyint

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
* XDY_Mot 		Ordre_In
* XDY_District		District_In
* XDY_Nom		Libelle_In
* XDY_Remarque		Remarque_In
* XDY_Octet		Gravite
* XDY_Octet		Trafic
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
* Ajoute dans la table SAD_EXP
*
* MECANISMES :
*
------------------------------------------------------*/


as 
   
   declare	@vl_evt int,
   			@vl_cle tinyint,
   			@vl_Status int,
			@vl_Site tinyint

#ifdef HIST

	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAP;23
			@NumPA_In		,
			@Ordre_In		,
			@District_In		,
			@Libelle_In		,
			@Remarque_In		,
			@Gravite_in		,
			@Trafic_in

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP;23
			@NumPA_In		,
			@Ordre_In		,
			@District_In		,
			@Libelle_In		,
			@Remarque_In		,
			@Gravite_in		,
			@Trafic_in

	if @vl_Status = null
		return XDC_PRC_INC
	else
		return @vl_Status

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
      ** Ajout dans la table SAD_EXP d'une proposition de
      ** "export intergestionnaire" a un plan d'actions
      */
   
      if @Remarque_In = null
         select @Remarque_In = ' '
      
      if @Libelle_In = null
         select @Libelle_In = ' '
      

      if not exists ( select * from EXP..ACT_GEN ACT, EXP..SAD_PLN SAD
			where SAD.numero=@NumPA_In and
				SAD.evenement=ACT.evenement and
				SAD.cle=ACT.cle and
				type=XDC_ACT_EXP and
				heure_fin = null )

	      	insert into EXP..SAD_EXP (numero, ordre, district, remarque, libelle, gravite, trafic )
      		values ( @NumPA_In, @Ordre_In, @District_In, @Remarque_In, @Libelle_In, @Gravite_in, @Trafic_in )
 

      return XDC_OK
      
   end
#endif
go
