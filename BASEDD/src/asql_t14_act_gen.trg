/*E*/
/*  Fichier : $Id: asql_t14_act_gen.trg,v 1.10 2020/03/10 09:19:07 gesconf Exp $      Release : $Revision: 1.10 $        Date : $Date: 2020/03/10 09:19:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  VEHEDD
------------------------------------------------------
* FICHIER asql_t14_act_gen.trg
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* trigger sur la table FMC_VEH
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.  15/11/94        : Creation
* C.T.  26/01/98        : Insertion dans FMC_ACT si evenement n'existe pas (1531)
* P.N   23/12/98	: modif interges dem/1723 
* JMG	02/03/04	: insertion dans ADA_ACT (SAGA) 1.4
* FL    05/05/04        : suppression des actions export DATEX (SAGA) 1.5
* F.Lizot  27/06/2005	:  Correction SAGA DEM473 1.6
* JMG	19/09/05	: suppression trace select en sortie DEM515 1.7
* PNI	13/12/2010	: Suppresion des actions PMV inserees dans ADA_ACT faisant doublon avec PMV_ADA 1.8
* PNI	19/11/12	: Correction pour le pmv en echec 1.8
* PNI	06/01/16	: Suppression des insert dans ADA_ACT pour ne plus envoyer les actions v1.9 DEM1145
* PNI	10/03/20	: Ajout mise a jour SYN_OBJ_POR suite a repli MSA v1.10
* GGY	15/01/24	: Ajout appel AARP02 pour envoi msg XDM_AS_UPDATE_PA (DEM568)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* met a jour la colonne version pour garantir la coherence
* en cas de retard de replication
*
* Code retour
* 
* neant
*
* Conditions d'utilisation
* base EXP 
---------------------------------------------------*/

use EXP
go

drop trigger act_gen_iu_trg
go

create trigger act_gen_iu_trg
on ACT_GEN
for insert,update
as

/*A Declaration des variables locales */

	declare @vl_valeur int,
		@vl_evenement int,
		@vl_action int,
		@vl_cle tinyint,
		@vl_type tinyint,
		@vl_numero int,
		@vl_numero2 int,
		@vl_sit tinyint,
		@vl_serveurNumero tinyint

/*A Recuperation de la valeur precedente et mise a jour */
	select @vl_valeur=valeur
	from FMC_ACT, inserted
	where FMC_ACT.numero=inserted.evenement and
	      FMC_ACT.cle=inserted.cle
	
	if @@rowcount=0
	insert FMC_ACT (numero,cle,valeur)
	select distinct evenement,cle,1 from inserted
	
	else
	update FMC_ACT
	set valeur=isnull(@vl_valeur,0)+1
	from inserted
	where FMC_ACT.numero=inserted.evenement and
	      FMC_ACT.cle=inserted.cle

#ifdef CI

	insert PMV_ADA 
	select inserted.numero,sit 
	from inserted , CFG..ADA_PMV 
	where type=XDC_ACT_PMV and inserted.equipement=CFG..ADA_PMV.numero and 
		(heure_echec=null or heure_succes != null) and 
	(heure_succes != null or heure_fin != null)
#endif

/*SAGA*/
#ifdef CI
	select @vl_evenement = evenement, @vl_cle = cle, @vl_type = type,
		@vl_numero = numero, @vl_sit = sit
	from inserted
	where type!=XDC_ACT_PMV

	/* Recherche si la FMC associee est exportee (ou exportable) */
/*DEM1145        select @vl_numero2 = G.numero
	from EXP..FMC_GEN G, EXP..FMC_HIS_DER D, CFG..ADA_EVT E
	where G.numero = D.numero and G.cle = D.cle
	and D.autoroute=E.autoroute and G.type=E.type
	and D.PR <= E.fin and D.PR >= E.debut
	and G.numero = @vl_evenement and G.cle = @vl_cle
	and @vl_numero not in (select actio from EXP..ADA_ACT where sit = @vl_sit)
*/
	/*si on a trouve une action*/
/*DEM1145	if @@rowcount!=0
	begin
		insert ADA_ACT values (@vl_numero, @vl_sit)
	end
*/
#endif

	select @vl_numero= inserted.numero,@vl_sit=inserted.sit from inserted where inserted.heure_fin!=null

	if @@rowcount!=0
		update EXP..SYN_OBJ_POR set ordre=3 where actio=@vl_numero and cle_action=@vl_sit

/* Envoi du message XDM_AS_UPDATE_PA */
		
	select	@vl_numero=inserted.evenement,
			@vl_cle=inserted.cle,
			@vl_action=inserted.numero,
			@vl_sit=inserted.sit,
			@vl_valeur=FMC_ACT.valeur
				from FMC_ACT, inserted
						where	FMC_ACT.numero=inserted.evenement and
								FMC_ACT.cle=inserted.cle

	if @vl_action > 0
	begin
		declare serveursNom cursor
			
			for select numero from CFG..RES_PRA where serveur_pra = @@servername

		open  serveursNom
		fetch serveursNom into @vl_serveurNumero

			while (@@sqlstatus=0)
			begin
				if @vl_serveurNumero = XDC_VC
					exec TASRV_VC...AARP02 	@vl_numero, 
											@vl_cle, 
											@vl_action, 
											@vl_valeur

				else if @vl_serveurNumero = XDC_DP
					exec TASRV_DP...AARP02 	@vl_numero, 
											@vl_cle, 
											@vl_action, 
											@vl_valeur

				else if @vl_serveurNumero = XDC_CI
					exec TASRV_CI...AARP02 	@vl_numero, 
											@vl_cle, 
											@vl_action, 
											@vl_valeur

				else if @vl_serveurNumero = XDC_CA
					exec TASRV_CA...AARP02	@vl_numero, 
											@vl_cle, 
											@vl_action, 
											@vl_valeur

			fetch serveursNom into @vl_serveurNumero
			end
		close serveursNom
	end
go
