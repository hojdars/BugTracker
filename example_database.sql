--
-- PostgreSQL database dump
--

-- Dumped from database version 9.4.5
-- Dumped by pg_dump version 9.4.5
-- Started on 2016-04-19 18:05:19

SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

DROP DATABASE bugtracker_database;
--
-- TOC entry 2014 (class 1262 OID 16506)
-- Name: bugtracker_database; Type: DATABASE; Schema: -; Owner: postgres
--

CREATE DATABASE bugtracker_database WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'Czech_Czech Republic.1250' LC_CTYPE = 'Czech_Czech Republic.1250';


ALTER DATABASE bugtracker_database OWNER TO postgres;

\connect bugtracker_database

SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

--
-- TOC entry 6 (class 2615 OID 2200)
-- Name: public; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA public;


ALTER SCHEMA public OWNER TO postgres;

--
-- TOC entry 2015 (class 0 OID 0)
-- Dependencies: 6
-- Name: SCHEMA public; Type: COMMENT; Schema: -; Owner: postgres
--

COMMENT ON SCHEMA public IS 'standard public schema';


--
-- TOC entry 176 (class 3079 OID 11855)
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- TOC entry 2017 (class 0 OID 0)
-- Dependencies: 176
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- TOC entry 172 (class 1259 OID 16507)
-- Name: bugstates; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE bugstates (
    "ID" integer NOT NULL,
    state_name character varying(40)
);


ALTER TABLE bugstates OWNER TO postgres;

--
-- TOC entry 173 (class 1259 OID 16510)
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
-- TOC entry 174 (class 1259 OID 16516)
-- Name: columns; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE columns (
    "ID" integer NOT NULL,
    column_name character varying(40)
);


ALTER TABLE columns OWNER TO postgres;

--
-- TOC entry 175 (class 1259 OID 16519)
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
-- TOC entry 2018 (class 0 OID 0)
-- Dependencies: 175
-- Name: foo_a_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE foo_a_seq OWNED BY bugtable.id;


--
-- TOC entry 1890 (class 2604 OID 16521)
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY bugtable ALTER COLUMN id SET DEFAULT nextval('foo_a_seq'::regclass);


--
-- TOC entry 2006 (class 0 OID 16507)
-- Dependencies: 172
-- Data for Name: bugstates; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO bugstates ("ID", state_name) VALUES (1, 'Important');
INSERT INTO bugstates ("ID", state_name) VALUES (2, 'Normal');
INSERT INTO bugstates ("ID", state_name) VALUES (3, 'Solved');
INSERT INTO bugstates ("ID", state_name) VALUES (5, 'Zelenej!');
INSERT INTO bugstates ("ID", state_name) VALUES (11, 'Modrej.');


--
-- TOC entry 2007 (class 0 OID 16510)
-- Dependencies: 173
-- Data for Name: bugtable; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO bugtable (id, name, author, date, importance, description, tested, showdown) VALUES (1, 'Run to the Hills', 'Iron Maiden', '1223-01-01', 2, 'About indians.', true, 1);
INSERT INTO bugtable (id, name, author, date, importance, description, tested, showdown) VALUES (2, 'Trooper', 'Iron Maiden', '1995-10-01', 1, 'The famous riffs!', false, 11);
INSERT INTO bugtable (id, name, author, date, importance, description, tested, showdown) VALUES (3, 'Fear of the Dark', 'Iron Maiden', '1995-01-10', 3, '...are you watching me?', true, 1);
INSERT INTO bugtable (id, name, author, date, importance, description, tested, showdown) VALUES (5, 'Lonely Day', 'System of a Down', '2001-07-08', 2, 'The most loneliest day of my life...', false, 2);
INSERT INTO bugtable (id, name, author, date, importance, description, tested, showdown) VALUES (4, 'Breaking the Law', 'Judas Priest', '2000-12-13', 1, 'Breaking the law, breaking the law <3', true, 3);
INSERT INTO bugtable (id, name, author, date, importance, description, tested, showdown) VALUES (6, 'Sorry', 'Justin Bieber', '2015-01-10', 5, 'No comment. This is a BUG.', true, 11);


--
-- TOC entry 2008 (class 0 OID 16516)
-- Dependencies: 174
-- Data for Name: columns; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO columns ("ID", column_name) VALUES (1, 'ID');
INSERT INTO columns ("ID", column_name) VALUES (2, 'Name');
INSERT INTO columns ("ID", column_name) VALUES (3, 'Author');
INSERT INTO columns ("ID", column_name) VALUES (4, 'Date');
INSERT INTO columns ("ID", column_name) VALUES (5, 'Importance');
INSERT INTO columns ("ID", column_name) VALUES (6, 'Description');
INSERT INTO columns ("ID", column_name) VALUES (7, 'Tested');
INSERT INTO columns ("ID", column_name) VALUES (8, 'Showdown');


--
-- TOC entry 2019 (class 0 OID 0)
-- Dependencies: 175
-- Name: foo_a_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('foo_a_seq', 6, true);


--
-- TOC entry 1896 (class 2606 OID 16523)
-- Name: IDNum; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY columns
    ADD CONSTRAINT "IDNum" PRIMARY KEY ("ID");


--
-- TOC entry 1892 (class 2606 OID 16525)
-- Name: idkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY bugstates
    ADD CONSTRAINT idkey PRIMARY KEY ("ID");


--
-- TOC entry 1894 (class 2606 OID 16527)
-- Name: lidi_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY bugtable
    ADD CONSTRAINT lidi_pkey PRIMARY KEY (id);


--
-- TOC entry 2016 (class 0 OID 0)
-- Dependencies: 6
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


-- Completed on 2016-04-19 18:05:19

--
-- PostgreSQL database dump complete
--

