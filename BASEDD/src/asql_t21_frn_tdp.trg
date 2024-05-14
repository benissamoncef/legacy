/*E*/
/*  Fichier : $Id: asql_t21_frn_tdp.trg,v 1.4 2000/12/11 16:24:14 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 2000/12/11 16:24:14 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER asql_t21_frn_tdp.trg
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* trigger sur la table FRN_TDP
*
------------------------------------------------------
* HISTORIQUE :
*
* niepceron  	03 Mar 97        : Creation
* niepceron  	03 Mar 97        : Creation 1.2
* niepceron	14 Mai 98	 : Les reveils sur FMC op vont uniquement cu CI DEM/1661
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Cree un reveil et previent  l'IHM sur insert ou update
*
* Code retour
* 
* nEant
*
* Conditions d'utilisation
* base EXP
---------------------------------------------------*/

use EXP
go

drop trigger frn_tdp_iu_trg
go

create trigger frn_tdp_iu_trg
on FRN_TDP
for insert,update
as

   /*A 
   ** DEclaration des variables locales 
   */

   declare      @vl_site_courant        tinyint,
                @vl_Site                tinyint,
                @vl_siteCI              tinyint,
                @vl_NumEvt              int,
                @vl_CleEvt              tinyint,
                @vl_PosteOperateur      smallint,
                @vl_TexteReveil         char(25),
                @vl_NomMachine          char(10),
                @vl_Texte               char(100),
                @vl_NomSite             char(2),
                @vl_Type                tinyint,
                @vl_poste               smallint,
                @vl_zdp                 smallint,
                @vl_site_zdp            tinyint,
                @vl_Debut               datetime,
                @vl_Fin                 datetime
                
/* initialisation variable locale */
select @vl_siteCI = numero from CFG..RES_DIS where type = XDC_TYPEM_SD

/*A Rechercher le site courant */
        if @@servername = XDC_SQLCI
                select @vl_site_courant = XDC_CI
        else if @@servername = XDC_SQLDP
                select @vl_site_courant = XDC_DP
        else if @@servername = XDC_SQLVC
                select @vl_site_courant = XDC_VC
        else if @@servername = XDC_SQLCA
                select @vl_site_courant = XDC_CA


declare fmc cursor for
select distinct
                no_fmc,
                cle_fmc, 
                zdp,
                site_zdp ,
                debut,
                fin 
from inserted
   
open fmc
fetch  fmc into
                @vl_NumEvt,
                @vl_CleEvt, 
                @vl_zdp,
                @vl_site_zdp,
                @vl_Debut,
                @vl_Fin 
   
   
while (@@sqlstatus = 0)
begin
   /* recherche type et site de la FMC */
   select @vl_PosteOperateur=poste_enrichisseur ,
          @vl_Type=type,
          @vl_Site=sit
   from EXP..FMC_GEN
   where numero=@vl_NumEvt and cle=@vl_CleEvt
  

  /*A On envoie le reveil uniquement si le site enrichisseur est le site courant */
  if      (@vl_Site= @vl_site_courant)
  begin
   /* si seuil haut ... */
   if @vl_Fin=null
   begin
        /* Pas traitement pour FMC Operateur (traite dans XZAD01) */
        if @vl_Type<>XZAEC_FMC_PosteOperateur 
        begin
                /* envoie le reveil sur tous les poste du site ayant la FMC dans la liste des FMC a traiter */
                /* creation du texte du reveil */
                select 
                        @vl_Texte="DEPASSEMENT SEUIL TDP SUR ZONE "+rtrim(ZDP.libelle)+" AUT. "+rtrim(AUT.nom)+
                                " SENS "+convert(char(1),GEN.sens)+
                                " ENTRE PR "+convert(char(7),convert(decimal(9,3),GEN.PR)/1000)+
                                " ET "+convert(char(7),convert(decimal(9,3),ZDP.PR_fin)/1000)
                from CFG..EQT_GEN GEN,CFG..EQT_ZDP ZDP, CFG..RES_AUT AUT
                where   GEN.numero=@vl_zdp and 
                        GEN.numero=ZDP.numero and
                        GEN.type=XDC_EQT_ZDP and
                        AUT.numero=GEN.autoroute
                        
                /* !!!!! On utilise un curseur car le triggeur sur fmc_rev ne prend en compte
                        que la derniere ligne d'un update */
                        
                declare postes cursor for
                select  poste_enrichisseur 
                from EXP..FMC_GEN 
                where   type=XZAEC_FMC_PosteOperateur and
                        sit=@vl_Site and
                        fin=null
                        
                open postes
                fetch postes into @vl_poste
                while (@@sqlstatus = 0)
                begin
                        insert EXP..FMC_REV
                              ( evenement, cle, machine_operateur, texte, sit )
                        values (@vl_NumEvt, @vl_CleEvt,  @vl_poste,  @vl_Texte, @vl_Site)
                        fetch  postes into @vl_poste
                end
                close postes
                deallocate cursor postes

        end
        
   end
   /* envoie de reveil sur seuil bas */
   else
   begin
        /* Si la FMC a pilote un PMV sur reveil de seuil haut ou
                s'il s'agit d'une FMC BOUCHON alors on envoie le reveil */
        if exists  ( select * 
                        from EXP..ACT_GEN GEN,EXP..ACT_PMV PMV
                        where   GEN.evenement=@vl_NumEvt and
                                GEN.cle=@vl_CleEvt and
                                GEN.type=XDC_ACT_PMV and
                                GEN.heure_fin=null and
                                GEN.numero=PMV.actio and
                                PMV.depass_seuil=XDC_VRAI )     or
           /*@vl_Type=XZAEC_FMC_PosteOperateur*/
           @vl_Type=XZAEC_FMC_QueueBouchon
        begin
                /* creation du texte du reveil */
                select 
                        @vl_Texte="RETOUR TDP NORMAL SUR ZONE "+rtrim(ZDP.libelle)+" AUT. "+rtrim(AUT.nom)+
                                " SENS "+convert(char(1),GEN.sens)+
                                " ENTRE PR "+convert(char(7),convert(decimal(9,3),GEN.PR)/1000)+
                                " ET "+convert(char(7),convert(decimal(9,3),ZDP.PR_fin)/1000)
                from CFG..EQT_GEN GEN,CFG..EQT_ZDP ZDP, CFG..RES_AUT AUT
                where   GEN.numero=@vl_zdp and 
                        GEN.numero=ZDP.numero and
                        GEN.type=XDC_EQT_ZDP and
                        AUT.numero=GEN.autoroute
                        
                        
                /* !!!!! On utilise un curseur car le trigger sur fmc_rev ne prend en compte
                        que la derniere ligne d'un update */

                /* Si reveil sur FMV Operateur, alors uniquement sur celle du poste */
                /*if  @vl_Type!=XZAEC_FMC_PosteOperateur*/
                if  @vl_Type!=XZAEC_FMC_QueueBouchon
                begin
                        /* envoie le reveil sur tous les poste du site ayant la FMC dans la liste des FMC a traiter */
                        declare postes cursor for
                        select  poste_enrichisseur , sit
                        from EXP..FMC_GEN 
                        where   type=XZAEC_FMC_PosteOperateur and
                                sit=@vl_Site and
                                fin=null
                end
                else
                begin
                        /* envoie le reveil sur la FMC BOUCHON en local et au CI */
                        
                        declare postes cursor for
                        select  poste_enrichisseur, sit 
                        from EXP..FMC_GEN 
                        where   type=XZAEC_FMC_PosteOperateur and
                                (sit=@vl_Site  or sit = @vl_siteCI ) and
                                fin=null
                end
                        
                open postes
                fetch postes into @vl_poste, @vl_Site
                while (@@sqlstatus = 0)
                begin
                        insert EXP..FMC_REV
                              ( evenement, cle, machine_operateur, texte, sit )
                        values (@vl_NumEvt, @vl_CleEvt,  @vl_poste,  @vl_Texte, @vl_Site)
                        fetch  postes into @vl_poste, @vl_Site
                end
                close postes
                deallocate cursor postes

        end
   end
   end
fetch  fmc into
                @vl_NumEvt,
                @vl_CleEvt, 
                @vl_zdp,
                @vl_site_zdp,
                @vl_Debut,
                @vl_Fin 
end

close fmc

go
