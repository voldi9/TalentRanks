#!/bin/bash

createdb rankings
psql rankings -U oig2 < setupnewdb.sql