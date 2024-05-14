/*E*/
/*  Fichier : $Id: xzao12sp.prc,v 1.5 2018/07/11 09:26:08 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2018/07/11 09:26:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao12sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* SEQUENCE D'APPEL : 
*
* SP	XZAO12SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Nom		NomPlan_in
* XDY_Octet		NumEchSortie_in
* XDY_Octet		NumEchReinj_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Mot		NumPlan_out
* XDY_Entier		Resultat_out : XDC_OK,XDC_NOK=homonymie,XDC_ECHEC=couple entree-reinjection existant
*

* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION : BASE CFG du CI
*
* FONCTION :
*
* cree un plan de secours
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	04 jan 1995	: Creation (1.1)
* gaborit	29 jan 1995	: Modif site CI (1.2)
* Noel		06 Mar 1996	: Suppression du controle sur la creation de deux plans 
*				  ayant la meme paires d'�changeurs.
*					(DEM 1012) 		V1.3
* LCL		04/07/17	: modif PRA
* LCL		11/07/18	: correction syntaxe from
* GGY	26/09/23 :	Ajout district CA (DEM483)
------------------------------------------------------*/

 
use PRC

go

create procedure XZAO;12
   @va_NomPlan_in	char(25),
   @va_EchSor_in	tinyint,
   @va_EchReinj_in	tinyint,
   @va_NumPlan_out	smallint output,
   @va_Resultat_out	int output
   as
 declare
		@vl_Site tinyint
   
	/* Verification que le site est bien celui du CI ou CA */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

		/* Si le site n'est pas CI ou CA return NOK*/
		if @@rowcount <> 1
			return XDC_NOK
   
   /* Test sur les parametres d'entree */
   	   
   if not exists (select * from CFG..RES_ECH where numero=@va_EchSor_in)
   return XDC_ARG_INV
   	   
   if not exists (select * from CFG..RES_ECH where numero=@va_EchReinj_in)
   return XDC_ARG_INV
   
   select @va_Resultat_out=XDC_OK
   
   if exists (select * from CFG..PLN_SCR 
   		where nom=@va_NomPlan_in)
   select @va_Resultat_out=XDC_NOK
   
   if @va_Resultat_out<>XDC_OK
   return XDC_OK
   
   /*A Recuperation du numero de plan */
   select @va_NumPlan_out=max(numero) from CFG..PLN_SCR
   
   select @va_NumPlan_out=isnull(@va_NumPlan_out,0)+1 -- on incremente le numero du plan
   
   insert CFG..PLN_SCR values (
   				@va_NumPlan_out,
   				@va_EchSor_in,
   				@va_EchReinj_in,
   				@va_NomPlan_in
   				)
   if @@rowcount = 0 return XDC_NOK
   
   return XDC_OK
go            


