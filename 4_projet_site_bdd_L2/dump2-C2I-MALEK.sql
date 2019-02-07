--
-- PostgreSQL database dump
--

-- Dumped from database version 9.5.10
-- Dumped by pg_dump version 9.6.2

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET search_path = public, pg_catalog;

--
-- Name: get(character); Type: FUNCTION; Schema: public; Owner: amalek
--

CREATE FUNCTION get(model character) RETURNS integer
    LANGUAGE plpgsql
    AS $$ 
BEGIN 
RETURN (SELECT modulee FROM contient WHERE sesion = model); 
END;
$$;


ALTER FUNCTION public.get(model character) OWNER TO amalek;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: inscrit; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE inscrit (
    codesession integer NOT NULL,
    codepersonne integer NOT NULL
);


ALTER TABLE inscrit OWNER TO amalek;

--
-- Name: personne; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE personne (
    numpersonne integer NOT NULL,
    nom character(25) NOT NULL,
    prenom character(25) NOT NULL,
    mail character(60) NOT NULL,
    login character varying(20)
);


ALTER TABLE personne OWNER TO amalek;

--
-- Name: a1; Type: VIEW; Schema: public; Owner: amalek
--

CREATE VIEW a1 AS
 SELECT p.numpersonne,
    p.nom,
    p.prenom
   FROM (inscrit i
     JOIN personne p ON ((p.numpersonne = i.codepersonne)))
  WHERE (i.codesession = 1);


ALTER TABLE a1 OWNER TO amalek;

--
-- Name: a12; Type: VIEW; Schema: public; Owner: amalek
--

CREATE VIEW a12 AS
 SELECT p.numpersonne,
    p.nom,
    p.prenom
   FROM (inscrit i
     JOIN personne p ON ((p.numpersonne = i.codepersonne)))
  WHERE (i.codesession = 12);


ALTER TABLE a12 OWNER TO amalek;

--
-- Name: a13; Type: VIEW; Schema: public; Owner: amalek
--

CREATE VIEW a13 AS
 SELECT p.numpersonne,
    p.nom,
    p.prenom
   FROM (inscrit i
     JOIN personne p ON ((p.numpersonne = i.codepersonne)))
  WHERE (i.codesession = 13);


ALTER TABLE a13 OWNER TO amalek;

--
-- Name: a2; Type: VIEW; Schema: public; Owner: amalek
--

CREATE VIEW a2 AS
 SELECT p.numpersonne,
    p.nom,
    p.prenom
   FROM (inscrit i
     JOIN personne p ON ((p.numpersonne = i.codepersonne)))
  WHERE (i.codesession = 2);


ALTER TABLE a2 OWNER TO amalek;

--
-- Name: a3; Type: VIEW; Schema: public; Owner: amalek
--

CREATE VIEW a3 AS
 SELECT p.numpersonne,
    p.nom,
    p.prenom
   FROM (inscrit i
     JOIN personne p ON ((p.numpersonne = i.codepersonne)))
  WHERE (i.codesession = 3);


ALTER TABLE a3 OWNER TO amalek;

--
-- Name: a4; Type: VIEW; Schema: public; Owner: amalek
--

CREATE VIEW a4 AS
 SELECT p.numpersonne,
    p.nom,
    p.prenom
   FROM (inscrit i
     JOIN personne p ON ((p.numpersonne = i.codepersonne)))
  WHERE (i.codesession = 4);


ALTER TABLE a4 OWNER TO amalek;

--
-- Name: a6; Type: VIEW; Schema: public; Owner: amalek
--

CREATE VIEW a6 AS
 SELECT p.numpersonne,
    p.nom,
    p.prenom
   FROM (inscrit i
     JOIN personne p ON ((p.numpersonne = i.codepersonne)))
  WHERE (i.codesession = 6);


ALTER TABLE a6 OWNER TO amalek;

--
-- Name: a9; Type: VIEW; Schema: public; Owner: amalek
--

CREATE VIEW a9 AS
 SELECT p.numpersonne,
    p.nom,
    p.prenom
   FROM (inscrit i
     JOIN personne p ON ((p.numpersonne = i.codepersonne)))
  WHERE (i.codesession = 9);


ALTER TABLE a9 OWNER TO amalek;

--
-- Name: annee; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE annee (
    annee integer NOT NULL
);


ALTER TABLE annee OWNER TO amalek;

--
-- Name: annee_universitaire; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE annee_universitaire (
    annee_univ character(10) NOT NULL
);


ALTER TABLE annee_universitaire OWNER TO amalek;

--
-- Name: appartient; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE appartient (
    idsurv integer NOT NULL,
    numgrp integer NOT NULL
);


ALTER TABLE appartient OWNER TO amalek;

--
-- Name: authentification; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE authentification (
    login character(20) NOT NULL,
    motdepasse character(60) NOT NULL,
    type character(15) NOT NULL
);


ALTER TABLE authentification OWNER TO amalek;

--
-- Name: batiment; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE batiment (
    idbatiment integer NOT NULL,
    nombatiment character(50) NOT NULL,
    sousbatiment character(3) NOT NULL,
    gardien integer NOT NULL
);


ALTER TABLE batiment OWNER TO amalek;

--
-- Name: contient; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE contient (
    idsession integer NOT NULL,
    idmodule integer NOT NULL,
    note_min integer,
    CONSTRAINT contient_note_min_check CHECK (((note_min >= 0) AND (note_min <= 20)))
);


ALTER TABLE contient OWNER TO amalek;

--
-- Name: convoque; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE convoque (
    numpersonne integer NOT NULL,
    codeepreuve integer NOT NULL,
    idsalle integer NOT NULL
);


ALTER TABLE convoque OWNER TO amalek;

--
-- Name: corrige; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE corrige (
    idcorrecteur integer NOT NULL,
    codeepreuve integer NOT NULL,
    numpersonne integer NOT NULL,
    note integer NOT NULL,
    CONSTRAINT corrige_note_check CHECK (((note >= 0) AND (note <= 20)))
);


ALTER TABLE corrige OWNER TO amalek;

--
-- Name: date_ouverture_bat; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE date_ouverture_bat (
    iddate integer NOT NULL,
    date_heure_ouv timestamp without time zone NOT NULL,
    date_heure_ferm timestamp without time zone NOT NULL
);


ALTER TABLE date_ouverture_bat OWNER TO amalek;

--
-- Name: epreuve; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE epreuve (
    codeepreuve integer NOT NULL,
    idsession integer NOT NULL,
    idmodule integer NOT NULL,
    dateheure timestamp without time zone NOT NULL,
    salle integer
);


ALTER TABLE epreuve OWNER TO amalek;

--
-- Name: epreuve_codeepreuve_seq; Type: SEQUENCE; Schema: public; Owner: amalek
--

CREATE SEQUENCE epreuve_codeepreuve_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE epreuve_codeepreuve_seq OWNER TO amalek;

--
-- Name: epreuve_codeepreuve_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: amalek
--

ALTER SEQUENCE epreuve_codeepreuve_seq OWNED BY epreuve.codeepreuve;


--
-- Name: etudiant; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE etudiant (
    numpersonne integer NOT NULL,
    codeetudiant character(8) NOT NULL,
    idformation integer NOT NULL,
    annee_univ character(10) NOT NULL
);


ALTER TABLE etudiant OWNER TO amalek;

--
-- Name: exterieur; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE exterieur (
    numpersonne integer NOT NULL,
    nsiret character(8) NOT NULL,
    adresse character(50) NOT NULL,
    telephone character(10) NOT NULL
);


ALTER TABLE exterieur OWNER TO amalek;

--
-- Name: formation; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE formation (
    idformation integer NOT NULL,
    libelle character(18) NOT NULL
);


ALTER TABLE formation OWNER TO amalek;

--
-- Name: gardien; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE gardien (
    idgardien integer NOT NULL,
    nom character(25) NOT NULL,
    prenom character(25) NOT NULL
);


ALTER TABLE gardien OWNER TO amalek;

--
-- Name: grpsurveillant; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE grpsurveillant (
    numgrp integer NOT NULL,
    numresponsable integer NOT NULL,
    idsalle integer NOT NULL,
    codeepreuve integer NOT NULL
);


ALTER TABLE grpsurveillant OWNER TO amalek;

--
-- Name: grpsurveillant_numgrp_seq; Type: SEQUENCE; Schema: public; Owner: amalek
--

CREATE SEQUENCE grpsurveillant_numgrp_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE grpsurveillant_numgrp_seq OWNER TO amalek;

--
-- Name: grpsurveillant_numgrp_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: amalek
--

ALTER SEQUENCE grpsurveillant_numgrp_seq OWNED BY grpsurveillant.numgrp;


--
-- Name: module_supp; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE module_supp (
    idmodule integer NOT NULL
);


ALTER TABLE module_supp OWNER TO amalek;

--
-- Name: modulee; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE modulee (
    idmodule integer NOT NULL,
    libelle character(25) NOT NULL
);


ALTER TABLE modulee OWNER TO amalek;

--
-- Name: modulee_idmodule_seq; Type: SEQUENCE; Schema: public; Owner: amalek
--

CREATE SEQUENCE modulee_idmodule_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE modulee_idmodule_seq OWNER TO amalek;

--
-- Name: modulee_idmodule_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: amalek
--

ALTER SEQUENCE modulee_idmodule_seq OWNED BY modulee.idmodule;


--
-- Name: moyen_ouverture; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE moyen_ouverture (
    idbatiment integer NOT NULL,
    iddate integer NOT NULL,
    moyen_ouverture character(20) NOT NULL
);


ALTER TABLE moyen_ouverture OWNER TO amalek;

--
-- Name: personne_numpersonne_seq; Type: SEQUENCE; Schema: public; Owner: amalek
--

CREATE SEQUENCE personne_numpersonne_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE personne_numpersonne_seq OWNER TO amalek;

--
-- Name: personne_numpersonne_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: amalek
--

ALTER SEQUENCE personne_numpersonne_seq OWNED BY personne.numpersonne;


--
-- Name: peut_corriger; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE peut_corriger (
    idcorrecteur integer NOT NULL,
    idmodule integer NOT NULL
);


ALTER TABLE peut_corriger OWNER TO amalek;

--
-- Name: pratique; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE pratique (
    num integer NOT NULL,
    code_loisir character(3) NOT NULL
);


ALTER TABLE pratique OWNER TO amalek;

--
-- Name: provient_service; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE provient_service (
    numpersonne integer NOT NULL,
    libelle character(20) NOT NULL
);


ALTER TABLE provient_service OWNER TO amalek;

--
-- Name: salle; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE salle (
    idsalle integer NOT NULL,
    nomsalle character(5) NOT NULL,
    capacite integer NOT NULL,
    batiment integer NOT NULL
);


ALTER TABLE salle OWNER TO amalek;

--
-- Name: service_admini; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE service_admini (
    libelle character(20) NOT NULL
);


ALTER TABLE service_admini OWNER TO amalek;

--
-- Name: session; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE session (
    idsession integer NOT NULL,
    nom character(20) NOT NULL,
    datedeb timestamp without time zone NOT NULL,
    datefin timestamp without time zone
);


ALTER TABLE session OWNER TO amalek;

--
-- Name: session_idsession_seq; Type: SEQUENCE; Schema: public; Owner: amalek
--

CREATE SEQUENCE session_idsession_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE session_idsession_seq OWNER TO amalek;

--
-- Name: session_idsession_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: amalek
--

ALTER SEQUENCE session_idsession_seq OWNED BY session.idsession;


--
-- Name: surveillant_correcteur; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE surveillant_correcteur (
    idsurv_cor integer NOT NULL,
    nom character(25) NOT NULL,
    prenom character(25) NOT NULL,
    telephone character(10) NOT NULL,
    numcomptebancaire character(19) NOT NULL,
    login character varying(20)
);


ALTER TABLE surveillant_correcteur OWNER TO amalek;

--
-- Name: surveillant_correcteur_idsurv_cor_seq; Type: SEQUENCE; Schema: public; Owner: amalek
--

CREATE SEQUENCE surveillant_correcteur_idsurv_cor_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE surveillant_correcteur_idsurv_cor_seq OWNER TO amalek;

--
-- Name: surveillant_correcteur_idsurv_cor_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: amalek
--

ALTER SEQUENCE surveillant_correcteur_idsurv_cor_seq OWNED BY surveillant_correcteur.idsurv_cor;


--
-- Name: valide_module; Type: TABLE; Schema: public; Owner: amalek
--

CREATE TABLE valide_module (
    numpersonne integer NOT NULL,
    idsession integer NOT NULL,
    idmodule integer NOT NULL
);


ALTER TABLE valide_module OWNER TO amalek;

--
-- Name: epreuve codeepreuve; Type: DEFAULT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY epreuve ALTER COLUMN codeepreuve SET DEFAULT nextval('epreuve_codeepreuve_seq'::regclass);


--
-- Name: grpsurveillant numgrp; Type: DEFAULT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY grpsurveillant ALTER COLUMN numgrp SET DEFAULT nextval('grpsurveillant_numgrp_seq'::regclass);


--
-- Name: modulee idmodule; Type: DEFAULT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY modulee ALTER COLUMN idmodule SET DEFAULT nextval('modulee_idmodule_seq'::regclass);


--
-- Name: personne numpersonne; Type: DEFAULT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY personne ALTER COLUMN numpersonne SET DEFAULT nextval('personne_numpersonne_seq'::regclass);


--
-- Name: session idsession; Type: DEFAULT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY session ALTER COLUMN idsession SET DEFAULT nextval('session_idsession_seq'::regclass);


--
-- Name: surveillant_correcteur idsurv_cor; Type: DEFAULT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY surveillant_correcteur ALTER COLUMN idsurv_cor SET DEFAULT nextval('surveillant_correcteur_idsurv_cor_seq'::regclass);


--
-- Data for Name: annee; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY annee (annee) FROM stdin;
2016
\.


--
-- Data for Name: annee_universitaire; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY annee_universitaire (annee_univ) FROM stdin;
L1        
L2        
\.


--
-- Data for Name: appartient; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY appartient (idsurv, numgrp) FROM stdin;
\.


--
-- Data for Name: authentification; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY authentification (login, motdepasse, type) FROM stdin;
                    	d41d8cd98f00b204e9800998ecf8427e                            	               
prof                	d450c5dbcc10db0749277efc32f15f9f                            	correcteur     
eleve               	c16da4ad70df593520193184381b9f21                            	etudiant       
toto                	f71dbe52628a3f83a77ab494817525c6                            	personnel      
nono                	c625ade02c3acde8e4d9de57fca78203                            	etudiant       
lolo                	d6581d542c7eaf801284f084478b5fcc                            	etudiant       
\.


--
-- Data for Name: batiment; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY batiment (idbatiment, nombatiment, sousbatiment, gardien) FROM stdin;
1	Grand-Batiment                                    	A  	1
2	Grand-Batiment                                    	B  	1
\.


--
-- Data for Name: contient; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY contient (idsession, idmodule, note_min) FROM stdin;
12	1	5
12	2	5
12	3	5
12	4	5
12	5	5
12	6	5
12	7	5
12	8	5
12	9	5
12	10	5
13	3	5
13	4	5
13	5	5
13	6	5
13	7	5
13	8	5
13	9	5
13	10	5
13	11	5
13	1	5
\.


--
-- Data for Name: convoque; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY convoque (numpersonne, codeepreuve, idsalle) FROM stdin;
\.


--
-- Data for Name: corrige; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY corrige (idcorrecteur, codeepreuve, numpersonne, note) FROM stdin;
15	51	1	12
15	51	2	12
15	52	1	12
15	53	1	12
15	54	1	18
15	55	1	11
15	56	1	10
15	53	5	12
15	61	5	1
15	62	5	11
15	63	5	12
15	64	5	18
15	65	5	18
15	66	5	8
15	67	5	14
15	68	5	16
15	69	5	17
15	70	5	9
15	61	35	11
15	62	35	11
15	63	35	11
15	64	35	12
15	65	35	14
15	66	35	18
15	67	35	18
15	68	35	16
15	69	35	17
15	70	35	14
15	54	2	16
15	54	5	13
15	54	35	17
15	62	1	2
15	62	2	7
15	62	4	2
15	62	3	4
15	63	2	5
15	61	3	5
\.


--
-- Data for Name: date_ouverture_bat; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY date_ouverture_bat (iddate, date_heure_ouv, date_heure_ferm) FROM stdin;
1	2016-03-20 08:00:00	2016-03-24 20:00:00
2	2016-03-25 08:00:00	2016-03-26 20:00:00
\.


--
-- Data for Name: epreuve; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY epreuve (codeepreuve, idsession, idmodule, dateheure, salle) FROM stdin;
51	12	1	2018-01-01 08:00:00	1
52	12	2	2018-01-01 10:00:00	1
53	12	3	2018-01-01 14:00:00	2
54	12	4	2018-01-01 16:00:00	3
55	12	5	2018-01-02 08:00:00	1
56	12	6	2018-01-02 10:00:00	2
57	12	7	2018-01-02 14:00:00	2
58	12	8	2018-01-02 16:00:00	1
59	12	9	2018-01-03 08:00:00	3
60	12	10	2018-01-03 10:00:00	1
61	13	1	2018-02-01 08:00:00	3
62	13	3	2018-02-01 10:00:00	1
63	13	4	2018-02-01 14:00:00	3
64	13	5	2018-02-01 16:00:00	1
65	13	6	2018-02-02 08:00:00	2
66	13	7	2018-02-02 10:00:00	3
67	13	8	2018-02-02 14:00:00	1
68	13	9	2018-02-02 16:00:00	2
69	13	10	2018-02-03 08:00:00	3
70	13	11	2018-02-03 10:00:00	3
\.


--
-- Name: epreuve_codeepreuve_seq; Type: SEQUENCE SET; Schema: public; Owner: amalek
--

SELECT pg_catalog.setval('epreuve_codeepreuve_seq', 70, true);


--
-- Data for Name: etudiant; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY etudiant (numpersonne, codeetudiant, idformation, annee_univ) FROM stdin;
40	12365478	2	L2        
1	12567899	2	L2        
\.


--
-- Data for Name: exterieur; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY exterieur (numpersonne, nsiret, adresse, telephone) FROM stdin;
\.


--
-- Data for Name: formation; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY formation (idformation, libelle) FROM stdin;
1	Math-Info         
2	MASS              
3	IMI               
\.


--
-- Data for Name: gardien; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY gardien (idgardien, nom, prenom) FROM stdin;
1	Geant                    	Vert                     
\.


--
-- Data for Name: grpsurveillant; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY grpsurveillant (numgrp, numresponsable, idsalle, codeepreuve) FROM stdin;
\.


--
-- Name: grpsurveillant_numgrp_seq; Type: SEQUENCE SET; Schema: public; Owner: amalek
--

SELECT pg_catalog.setval('grpsurveillant_numgrp_seq', 1, false);


--
-- Data for Name: inscrit; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY inscrit (codesession, codepersonne) FROM stdin;
13	2
13	1
13	3
13	4
13	5
12	35
13	35
12	1
12	2
12	5
\.


--
-- Data for Name: module_supp; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY module_supp (idmodule) FROM stdin;
\.


--
-- Data for Name: modulee; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY modulee (idmodule, libelle) FROM stdin;
1	Mathématiques            
2	Physique                 
3	Philosophie              
4	Economie                 
5	Marketing                
6	Littérature              
7	Sociologie               
8	Art-Plastique            
9	Jardinnage               
10	Chant                    
11	Sport                    
12	Danse                    
\.


--
-- Name: modulee_idmodule_seq; Type: SEQUENCE SET; Schema: public; Owner: amalek
--

SELECT pg_catalog.setval('modulee_idmodule_seq', 1, false);


--
-- Data for Name: moyen_ouverture; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY moyen_ouverture (idbatiment, iddate, moyen_ouverture) FROM stdin;
1	1	avec une cle        
1	2	sur demande         
2	1	avec une carte      
\.


--
-- Data for Name: personne; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY personne (numpersonne, nom, prenom, mail, login) FROM stdin;
5	Loic                     	Masdon                   	martiezn@etud.edu                                           	eleve
2	Paul                     	Loin                     	martin@etud.edu                                             	\N
4	Jeanmi                   	Posti                    	zert@etud.edu                                               	\N
3	Tati                     	Poeoin                   	martin@etud.edu                                             	\N
35	Jean                     	Michelle                 	toto@etud.edu                                               	toto
40	Doli                     	Prane                    	nono@etud.edu                                               	nono
1	Jean                     	Louis                    	jean@etud.edu                                               	lolo
\.


--
-- Name: personne_numpersonne_seq; Type: SEQUENCE SET; Schema: public; Owner: amalek
--

SELECT pg_catalog.setval('personne_numpersonne_seq', 40, true);


--
-- Data for Name: peut_corriger; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY peut_corriger (idcorrecteur, idmodule) FROM stdin;
15	1
15	2
15	3
15	4
15	5
\.


--
-- Data for Name: pratique; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY pratique (num, code_loisir) FROM stdin;
5	pon
5	ped
15	des
15	ten
15	ski
\.


--
-- Data for Name: provient_service; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY provient_service (numpersonne, libelle) FROM stdin;
35	Secretariat         
\.


--
-- Data for Name: salle; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY salle (idsalle, nomsalle, capacite, batiment) FROM stdin;
1	A101 	30	1
2	A202 	20	1
3	B102 	20	2
\.


--
-- Data for Name: service_admini; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY service_admini (libelle) FROM stdin;
Secretariat         
Infirmerie          
\.


--
-- Data for Name: session; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY session (idsession, nom, datedeb, datefin) FROM stdin;
12	Janvier 2018        	2018-01-01 00:00:00	\N
13	Fevrier 2018        	2018-02-01 00:00:00	\N
\.


--
-- Name: session_idsession_seq; Type: SEQUENCE SET; Schema: public; Owner: amalek
--

SELECT pg_catalog.setval('session_idsession_seq', 13, true);


--
-- Data for Name: surveillant_correcteur; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY surveillant_correcteur (idsurv_cor, nom, prenom, telephone, numcomptebancaire, login) FROM stdin;
15	Jean                     	jean                     	0654123278	1234567891         	prof
\.


--
-- Name: surveillant_correcteur_idsurv_cor_seq; Type: SEQUENCE SET; Schema: public; Owner: amalek
--

SELECT pg_catalog.setval('surveillant_correcteur_idsurv_cor_seq', 15, true);


--
-- Data for Name: valide_module; Type: TABLE DATA; Schema: public; Owner: amalek
--

COPY valide_module (numpersonne, idsession, idmodule) FROM stdin;
35	13	1
35	13	3
35	13	4
35	13	5
35	13	6
35	13	7
35	13	8
35	13	9
35	13	10
35	13	11
1	12	4
2	12	4
5	12	4
35	12	4
5	13	3
2	13	3
5	13	4
2	13	4
3	13	1
\.


--
-- Name: annee annee_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY annee
    ADD CONSTRAINT annee_pk PRIMARY KEY (annee);


--
-- Name: annee_universitaire annee_universitaire_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY annee_universitaire
    ADD CONSTRAINT annee_universitaire_pk PRIMARY KEY (annee_univ);


--
-- Name: appartient appartient_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY appartient
    ADD CONSTRAINT appartient_pk PRIMARY KEY (idsurv, numgrp);


--
-- Name: authentification authentification_pkey; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY authentification
    ADD CONSTRAINT authentification_pkey PRIMARY KEY (login);


--
-- Name: batiment batiment_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY batiment
    ADD CONSTRAINT batiment_pk PRIMARY KEY (idbatiment);


--
-- Name: contient contient_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY contient
    ADD CONSTRAINT contient_pk PRIMARY KEY (idsession, idmodule);


--
-- Name: convoque convoque_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY convoque
    ADD CONSTRAINT convoque_pk PRIMARY KEY (numpersonne, codeepreuve);


--
-- Name: corrige corrige_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY corrige
    ADD CONSTRAINT corrige_pk PRIMARY KEY (idcorrecteur, codeepreuve, numpersonne);


--
-- Name: corrige corrige_uk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY corrige
    ADD CONSTRAINT corrige_uk UNIQUE (codeepreuve, numpersonne);


--
-- Name: date_ouverture_bat date_ouverture_bat_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY date_ouverture_bat
    ADD CONSTRAINT date_ouverture_bat_pk PRIMARY KEY (iddate);


--
-- Name: epreuve epreuve_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY epreuve
    ADD CONSTRAINT epreuve_pk PRIMARY KEY (codeepreuve);


--
-- Name: epreuve epreuve_uk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY epreuve
    ADD CONSTRAINT epreuve_uk UNIQUE (idsession, idmodule);


--
-- Name: etudiant etudiant_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY etudiant
    ADD CONSTRAINT etudiant_pk PRIMARY KEY (numpersonne);


--
-- Name: etudiant etudiant_uk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY etudiant
    ADD CONSTRAINT etudiant_uk UNIQUE (codeetudiant);


--
-- Name: exterieur exterieur_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY exterieur
    ADD CONSTRAINT exterieur_pk PRIMARY KEY (numpersonne);


--
-- Name: formation formation_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY formation
    ADD CONSTRAINT formation_pk PRIMARY KEY (idformation);


--
-- Name: gardien gardien_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY gardien
    ADD CONSTRAINT gardien_pk PRIMARY KEY (idgardien);


--
-- Name: grpsurveillant grpsurveillant_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY grpsurveillant
    ADD CONSTRAINT grpsurveillant_pk PRIMARY KEY (numgrp);


--
-- Name: grpsurveillant grpsurveillant_uk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY grpsurveillant
    ADD CONSTRAINT grpsurveillant_uk UNIQUE (codeepreuve, idsalle);


--
-- Name: inscrit inscrit_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY inscrit
    ADD CONSTRAINT inscrit_pk PRIMARY KEY (codesession, codepersonne);


--
-- Name: module_supp module_supp_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY module_supp
    ADD CONSTRAINT module_supp_pk PRIMARY KEY (idmodule);


--
-- Name: modulee modulee_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY modulee
    ADD CONSTRAINT modulee_pk PRIMARY KEY (idmodule);


--
-- Name: moyen_ouverture moyen_ouverture_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY moyen_ouverture
    ADD CONSTRAINT moyen_ouverture_pk PRIMARY KEY (idbatiment, iddate);


--
-- Name: personne personne_login_uk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY personne
    ADD CONSTRAINT personne_login_uk UNIQUE (login);


--
-- Name: personne personne_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY personne
    ADD CONSTRAINT personne_pk PRIMARY KEY (numpersonne);


--
-- Name: peut_corriger peut_corrige_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY peut_corriger
    ADD CONSTRAINT peut_corrige_pk PRIMARY KEY (idcorrecteur, idmodule);


--
-- Name: pratique pk_pratique; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY pratique
    ADD CONSTRAINT pk_pratique PRIMARY KEY (num, code_loisir);


--
-- Name: provient_service provient_service_numpersonne_key; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY provient_service
    ADD CONSTRAINT provient_service_numpersonne_key UNIQUE (numpersonne);


--
-- Name: provient_service provient_service_pkey; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY provient_service
    ADD CONSTRAINT provient_service_pkey PRIMARY KEY (numpersonne, libelle);


--
-- Name: salle salle_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY salle
    ADD CONSTRAINT salle_pk PRIMARY KEY (idsalle);


--
-- Name: salle salle_uk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY salle
    ADD CONSTRAINT salle_uk UNIQUE (nomsalle, batiment);


--
-- Name: service_admini service_admini_pkey; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY service_admini
    ADD CONSTRAINT service_admini_pkey PRIMARY KEY (libelle);


--
-- Name: session sesion_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY session
    ADD CONSTRAINT sesion_pk PRIMARY KEY (idsession);


--
-- Name: session session_nom_uk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY session
    ADD CONSTRAINT session_nom_uk UNIQUE (nom);


--
-- Name: surveillant_correcteur surveillant_cor_login_uk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY surveillant_correcteur
    ADD CONSTRAINT surveillant_cor_login_uk UNIQUE (login);


--
-- Name: surveillant_correcteur surveillant_correcteur_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY surveillant_correcteur
    ADD CONSTRAINT surveillant_correcteur_pk PRIMARY KEY (idsurv_cor);


--
-- Name: valide_module valide_module_pk; Type: CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY valide_module
    ADD CONSTRAINT valide_module_pk PRIMARY KEY (numpersonne, idsession, idmodule);


--
-- Name: appartient appartient_groupesurveillant_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY appartient
    ADD CONSTRAINT appartient_groupesurveillant_fk FOREIGN KEY (numgrp) REFERENCES grpsurveillant(numgrp);


--
-- Name: appartient appartient_surveillant_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY appartient
    ADD CONSTRAINT appartient_surveillant_fk FOREIGN KEY (idsurv) REFERENCES surveillant_correcteur(idsurv_cor);


--
-- Name: batiment batiment_gardien_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY batiment
    ADD CONSTRAINT batiment_gardien_fk FOREIGN KEY (gardien) REFERENCES gardien(idgardien);


--
-- Name: contient contient_modulee_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY contient
    ADD CONSTRAINT contient_modulee_fk FOREIGN KEY (idmodule) REFERENCES modulee(idmodule) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: contient contient_sesion_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY contient
    ADD CONSTRAINT contient_sesion_fk FOREIGN KEY (idsession) REFERENCES session(idsession) ON DELETE CASCADE;


--
-- Name: convoque convoque_codeepreuve_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY convoque
    ADD CONSTRAINT convoque_codeepreuve_fk FOREIGN KEY (codeepreuve) REFERENCES epreuve(codeepreuve);


--
-- Name: convoque convoque_personne_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY convoque
    ADD CONSTRAINT convoque_personne_fk FOREIGN KEY (numpersonne) REFERENCES personne(numpersonne);


--
-- Name: convoque convoque_salle_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY convoque
    ADD CONSTRAINT convoque_salle_fk FOREIGN KEY (idsalle) REFERENCES salle(idsalle);


--
-- Name: corrige corrige_epreuve_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY corrige
    ADD CONSTRAINT corrige_epreuve_fk FOREIGN KEY (codeepreuve) REFERENCES epreuve(codeepreuve);


--
-- Name: corrige corrige_numsurveillant_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY corrige
    ADD CONSTRAINT corrige_numsurveillant_fk FOREIGN KEY (idcorrecteur) REFERENCES surveillant_correcteur(idsurv_cor);


--
-- Name: corrige corrige_personne_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY corrige
    ADD CONSTRAINT corrige_personne_fk FOREIGN KEY (numpersonne) REFERENCES personne(numpersonne);


--
-- Name: epreuve epreuve_module_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY epreuve
    ADD CONSTRAINT epreuve_module_fk FOREIGN KEY (idmodule) REFERENCES modulee(idmodule) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: epreuve epreuve_salle_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY epreuve
    ADD CONSTRAINT epreuve_salle_fk FOREIGN KEY (salle) REFERENCES salle(idsalle);


--
-- Name: epreuve epreuve_session_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY epreuve
    ADD CONSTRAINT epreuve_session_fk FOREIGN KEY (idsession) REFERENCES session(idsession) ON DELETE CASCADE;


--
-- Name: etudiant etudiant_annee_univ_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY etudiant
    ADD CONSTRAINT etudiant_annee_univ_fk FOREIGN KEY (annee_univ) REFERENCES annee_universitaire(annee_univ);


--
-- Name: etudiant etudiant_idformation_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY etudiant
    ADD CONSTRAINT etudiant_idformation_fk FOREIGN KEY (idformation) REFERENCES formation(idformation);


--
-- Name: etudiant etudiant_numpersonne_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY etudiant
    ADD CONSTRAINT etudiant_numpersonne_fk FOREIGN KEY (numpersonne) REFERENCES personne(numpersonne);


--
-- Name: exterieur exterieur_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY exterieur
    ADD CONSTRAINT exterieur_fk FOREIGN KEY (numpersonne) REFERENCES personne(numpersonne);


--
-- Name: grpsurveillant grpsurveillant_epreuve_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY grpsurveillant
    ADD CONSTRAINT grpsurveillant_epreuve_fk FOREIGN KEY (codeepreuve) REFERENCES epreuve(codeepreuve);


--
-- Name: grpsurveillant grpsurveillant_responsable_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY grpsurveillant
    ADD CONSTRAINT grpsurveillant_responsable_fk FOREIGN KEY (numresponsable) REFERENCES surveillant_correcteur(idsurv_cor);


--
-- Name: grpsurveillant grpsurveillant_salle_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY grpsurveillant
    ADD CONSTRAINT grpsurveillant_salle_fk FOREIGN KEY (idsalle) REFERENCES salle(idsalle);


--
-- Name: inscrit inscrit_personne_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY inscrit
    ADD CONSTRAINT inscrit_personne_fk FOREIGN KEY (codepersonne) REFERENCES personne(numpersonne);


--
-- Name: inscrit inscrit_sesion_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY inscrit
    ADD CONSTRAINT inscrit_sesion_fk FOREIGN KEY (codesession) REFERENCES session(idsession);


--
-- Name: module_supp module_supp_idmodule_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY module_supp
    ADD CONSTRAINT module_supp_idmodule_fk FOREIGN KEY (idmodule) REFERENCES modulee(idmodule);


--
-- Name: moyen_ouverture moyen_ouverture_batiment_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY moyen_ouverture
    ADD CONSTRAINT moyen_ouverture_batiment_fk FOREIGN KEY (idbatiment) REFERENCES batiment(idbatiment);


--
-- Name: moyen_ouverture moyen_ouverture_date_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY moyen_ouverture
    ADD CONSTRAINT moyen_ouverture_date_fk FOREIGN KEY (iddate) REFERENCES date_ouverture_bat(iddate);


--
-- Name: personne personne_login_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY personne
    ADD CONSTRAINT personne_login_fk FOREIGN KEY (login) REFERENCES authentification(login);


--
-- Name: peut_corriger peut_corrige_correcteur_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY peut_corriger
    ADD CONSTRAINT peut_corrige_correcteur_fk FOREIGN KEY (idcorrecteur) REFERENCES surveillant_correcteur(idsurv_cor);


--
-- Name: peut_corriger peut_corrige_idmodule_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY peut_corriger
    ADD CONSTRAINT peut_corrige_idmodule_fk FOREIGN KEY (idmodule) REFERENCES modulee(idmodule) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: provient_service provient_service_libelle_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY provient_service
    ADD CONSTRAINT provient_service_libelle_fk FOREIGN KEY (libelle) REFERENCES service_admini(libelle);


--
-- Name: provient_service provient_service_numpers_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY provient_service
    ADD CONSTRAINT provient_service_numpers_fk FOREIGN KEY (numpersonne) REFERENCES personne(numpersonne) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: salle salle_batiment_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY salle
    ADD CONSTRAINT salle_batiment_fk FOREIGN KEY (batiment) REFERENCES batiment(idbatiment);


--
-- Name: surveillant_correcteur surveillant_cor_login_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY surveillant_correcteur
    ADD CONSTRAINT surveillant_cor_login_fk FOREIGN KEY (login) REFERENCES authentification(login);


--
-- Name: valide_module valide_module_idmodule_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY valide_module
    ADD CONSTRAINT valide_module_idmodule_fk FOREIGN KEY (idmodule) REFERENCES modulee(idmodule);


--
-- Name: valide_module valide_module_idsession_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY valide_module
    ADD CONSTRAINT valide_module_idsession_fk FOREIGN KEY (idsession) REFERENCES session(idsession) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: valide_module valide_module_numpersonne_fk; Type: FK CONSTRAINT; Schema: public; Owner: amalek
--

ALTER TABLE ONLY valide_module
    ADD CONSTRAINT valide_module_numpersonne_fk FOREIGN KEY (numpersonne) REFERENCES personne(numpersonne) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- PostgreSQL database dump complete
--

