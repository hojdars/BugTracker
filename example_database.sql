--
-- PostgreSQL database dump
--

-- Dumped from database version 9.4.5
-- Dumped by pg_dump version 9.4.5
-- Started on 2016-04-03 20:36:20

SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

--
-- TOC entry 2015 (class 1262 OID 16393)
-- Name: testdb; Type: DATABASE; Schema: -; Owner: postgres
--

CREATE DATABASE testdb WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'Czech_Czech Republic.1250' LC_CTYPE = 'Czech_Czech Republic.1250';


ALTER DATABASE testdb OWNER TO postgres;

\connect testdb

SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

--
-- TOC entry 176 (class 3079 OID 11855)
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- TOC entry 2018 (class 0 OID 0)
-- Dependencies: 176
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- TOC entry 174 (class 1259 OID 16412)
-- Name: bugstates; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE bugstates (
    "ID" integer NOT NULL,
    state_name character varying(40)
);


ALTER TABLE bugstates OWNER TO postgres;

--
-- TOC entry 172 (class 1259 OID 16394)
-- Name: bugtable; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE bugtable (
    id integer NOT NULL,
    name character varying(40),
    author character varying(50),
    date date,
    importance integer,
    description character varying(500),
    tested boolean,
    showdown integer
);


ALTER TABLE bugtable OWNER TO postgres;

--
-- TOC entry 173 (class 1259 OID 16404)
-- Name: columns; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE columns (
    "ID" integer NOT NULL,
    column_name character varying(40)
);


ALTER TABLE columns OWNER TO postgres;

--
-- TOC entry 175 (class 1259 OID 16423)
-- Name: foo_a_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE foo_a_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE foo_a_seq OWNER TO postgres;

--
-- TOC entry 2019 (class 0 OID 0)
-- Dependencies: 175
-- Name: foo_a_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE foo_a_seq OWNED BY bugtable.id;


--
-- TOC entry 1891 (class 2604 OID 16425)
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY bugtable ALTER COLUMN id SET DEFAULT nextval('foo_a_seq'::regclass);


--
-- TOC entry 2009 (class 0 OID 16412)
-- Dependencies: 174
-- Data for Name: bugstates; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY bugstates ("ID", state_name) FROM stdin;
1	Important
2	Normal
3	Solved
5	Zelenej!
11	Modrej.
\.


--
-- TOC entry 2007 (class 0 OID 16394)
-- Dependencies: 172
-- Data for Name: bugtable; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY bugtable (id, name, author, date, importance, description, tested, showdown) FROM stdin;
13	Kaja gott	Pokuser	2015-01-01	1	aaaaa	t	1
15	Testing concurrence.mmm	Já	2016-02-27	11	qqqq	t	2
6	Tento projekt neexistuje	JustSayin	2014-01-01	3	Projekt neexistuje, měl by se vytvořit.	f	11
18	Posledni	Tester	2016-03-11	1	Snad to faka.	t	11
4	Hroch snědl všechen med....	Hroch	2015-12-31	1	Omlouvám se, nechtěl jsem.	f	1
3	kaja gottttt	Anonym	2016-02-11	1	Nepřijde vám divné, že tady edituje Karel Gott?	f	5
11	Konecne	Prvni	2016-02-24	11	Konecne sem to sem dostal pres Add BUG!	t	11
16	Name	Author	2015-01-01	3	Description PPPPPPP	f	2
1	Včelka Mája...	Karel Gott	2015-02-10	2	Mája je v naší verzi sršeň, né včela. Napravit, obludy!	t	2
5	Slon v databázi	Hroch a spol.	2015-11-17	3	V databázi je slon?! Jmenuje se postgres, a je modrej...	t	1
14	Update Stavů	Štěpán	2016-02-25	11	Projekt se blíží ke zdárnému konci, to je fajn. Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.	t	11
12	Edit gottttttt	Edit	1900-01-01	5	Edit	f	2
19	INsert	Please	9999-01-10	11	qqon	f	1
20	Ahoj	Test	2016-04-03	11	Proč to neinsertnulo?	t	1
21	sqsq	asf	1195-01-01	11	cejel	t	2
2	vcelka	Rýpalovič	2016-02-10	2	Chybí háček nad č. Grammar Nazi united.	t	5
17	Name	Author	2001-01-01	1	Descriptioneeering	t	1
\.


--
-- TOC entry 2008 (class 0 OID 16404)
-- Dependencies: 173
-- Data for Name: columns; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY columns ("ID", column_name) FROM stdin;
1	ID
2	Name
3	Author
4	Date
5	Importance
6	Description
7	Tested
8	Showdown
\.


--
-- TOC entry 2020 (class 0 OID 0)
-- Dependencies: 175
-- Name: foo_a_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('foo_a_seq', 21, true);


--
-- TOC entry 1895 (class 2606 OID 16408)
-- Name: IDNum; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY columns
    ADD CONSTRAINT "IDNum" PRIMARY KEY ("ID");


--
-- TOC entry 1897 (class 2606 OID 16416)
-- Name: idkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY bugstates
    ADD CONSTRAINT idkey PRIMARY KEY ("ID");


--
-- TOC entry 1893 (class 2606 OID 16398)
-- Name: lidi_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY bugtable
    ADD CONSTRAINT lidi_pkey PRIMARY KEY (id);


--
-- TOC entry 2017 (class 0 OID 0)
-- Dependencies: 5
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


-- Completed on 2016-04-03 20:36:20

--
-- PostgreSQL database dump complete
--

