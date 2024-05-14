#include "/users/gesconf/migrazur/XDMICG/inc/xdc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaec.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaac.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzacc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzahc.h"
/*E*/
/*Fichier : @(#)xzap02sp.prc	1.10      Release : 1.10        Date : 09/22/98
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap02sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Sep 1994	: Creation
* volcic	15 Sep 1994	: Modification pour SAD (v1.2)
* gaborit	21 Sep 1994	: Rien (v1.3)
* gaborit	21 Sep 1994	: Rien (v1.4)
* volcic	23 Sep 1994	: Modification pour SAD (v1.5)
* volcic	13 Oct 1994	: Modification (v1.5)
* volcic	28 Oct 1994	: Modification reception chaine nulle (v1.6)
* volcic	28 Oct 1994	: Rien (v1.7)
* volcic	28 Oct 1994	: Modification reception chaine nulle (v1.8)
* niepceron	10 Mar 1997	: Ajout de destination et destinationAlternat (v1.9)
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.10
* VR		07 Nov 2011 : Ajout Destination Noeud (DEM/1014)
* JMG		20/06/17 convergence 
* JMG		03/12/19 : EQT_FIL DEM1347 1.13
* JMG		02/11/20 	cle double EQT_FIL 1.14
* GGY		04/05/23	: Augmentation taille chalp TextePicto_In et NomPicto_In (DEM-473)
------------------------------------------------------*/

use PRC
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une proposition d'affichage PMV a un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
drop proc XZAP02
go

create procedure XZAP02

	@NumPA_In		smallint,
	@Ordre_In		smallint,
	@District_In		tinyint,
	@Remarque_In		char(50),
	@PrioriteEvt_In		tinyint,
	@NoEqt_In		smallint,
	@Explication_In		char(250),
	@Texte1PMV_In		char(15),
	@Texte2PMV_In		char(15),
	@Texte3PMV_In		char(15),
	@Texte4PMV_In		char(15),
	@Texte5PMV_In		char(15),
	@Texte6PMV_In		char(15),
	@Libelle_In		char(25),
	@TextePicto_In		char(XDC_LGR_TXT_PICTO),
	@NomPicto_In		char(XDC_LGR_NOM_PICTO),
	@Destination_In		smallint,
	@DestinationAlternat_In	smallint,
	@DestinationNoeud_In smallint ,
	@Realisable_In		bit,
	@Clignotement_In	int 

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
* XDY_Mot 		Ordre_In
* XDY_District		District_In
* XDY_Phrase		Remarque_In
* XDY_Octet 		PrioriteEvt_In
* XDY_Eqt 		NoEqt_In
* XDY_Commentaire	Explication_In
* XDY_LignePMV		Texte1PMV_In
* XDY_LignePMV		Texte2PMV_In
* XDY_LignePMV		Texte3PMV_In
* XDY_LignePMV		Texte4PMV_In
* XDY_LignePMV		Texte5PMV_In
* XDY_LignePMV		Texte6PMV_In
* XDY_Nom		Libelle_In
* XDY_TxtPicto		TextePicto_In
* XDY_NomPicto		NomPicto_In
* XDY_Mot		@Destination_In
* XDY_Mot		@DestinationAlternat_In
* XDY_Mot		@DestinationNoeud_In
* XDY_Booleen		Realisable_In

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
* Ajoute dans la table PA_PMV et retourne le numero du PMV
* cree
*
* MECANISMES :
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzap02sp.h" */

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* static char *version = "@(#)xzap02sp.prc	1.1 08/25/94 : xzap02sp" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */

as 
declare
   @vl_Status int,
   @vl_num int,
   @vl_cle tinyint
   
#ifdef HIST
	exec @vl_Status = SQL_CI.PRC..XZAP02
	@NumPA_In		,
	@Ordre_In		,
	@District_In		,
	@Remarque_In		,
	@PrioriteEvt_In		,
	@NoEqt_In		,
	@Explication_In		,
	@Texte1PMV_In		,
	@Texte2PMV_In		,
	@Texte3PMV_In		,
	@Texte4PMV_In		,
	@Texte5PMV_In		,
	@Texte6PMV_In		,
	@Libelle_In		,
	@TextePicto_In		,
	@NomPicto_In		,
	@Destination_In		,
	@DestinationAlternat_In	,
	@DestinationNoeud_In,
	@Realisable_In,
	@Clignotement_In

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
	or @PrioriteEvt_In = null 
	or @NoEqt_In = null
	or @NomPicto_In = null 
	or @Realisable_In = null
      
      return XDC_NOK
   else   
      begin
     
      /*A
      ** Ajout dans la table SAD_PMV (PA_PMV) d'un nouvelle proposition d'affichage PMV
      ** a un plan d'actions
      */
      
      if @Remarque_In = null
         select @Remarque_In = ' '
        
      if @Explication_In = null
         select @Explication_In = ' '
         
      if @Texte1PMV_In = null
         select @Texte1PMV_In = ' '   
         
      if @Texte2PMV_In = null
         select @Texte2PMV_In = ' '   
         
      if @Texte3PMV_In = null
         select @Texte3PMV_In = ' '   
         
      if @Texte4PMV_In = null
         select @Texte4PMV_In = ' '   
         
      if @Texte5PMV_In = null
         select @Texte5PMV_In = ' '   
         
      if @Texte6PMV_In = null
         select @Texte6PMV_In = ' '   
         
      if @Libelle_In = null
         select @Libelle_In = ' '   
         
      if @TextePicto_In = null
         select @TextePicto_In = ' '   
         
      
      insert into EXP..SAD_PMV

      values ( @NumPA_In, @Ordre_In, @District_In, @NoEqt_In, @PrioriteEvt_In, @Remarque_In, @Explication_In, @Texte1PMV_In, @Texte2PMV_In, @Texte3PMV_In, @Texte4PMV_In, @Texte5PMV_In, @Texte6PMV_In, @NomPicto_In, @TextePicto_In, @Realisable_In, @Libelle_In ,@Destination_In, @DestinationAlternat_In, @DestinationNoeud_In, @Clignotement_In)
   
      select @vl_num = evenement,
      		@vl_cle = cle
	from EXP..SAD_PLN
	where numero = @NumPA_In

      if @Libelle_In!= "EXTINCTION" and @Libelle_In!= "SAPIENS"
      begin
      	delete EXP..EQT_FIL
      	where evenement = @vl_num and cle = @vl_cle
      	and type_equipement = XDC_EQT_PMV
      	and equipement = @NoEqt_In
      	and sit = @District_In

      	insert into EXP..EQT_FIL
      	values (@vl_num, @vl_cle, XDC_EQT_PMV, @NoEqt_In, @PrioriteEvt_In, 0, @District_In)
      end

      return XDC_OK
      
      end
#endif
go
