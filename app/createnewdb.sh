#!/bin/bash

createdb rankings -U oig2 -O oig2
psql rankings -U oig2 < setupnewdb.sql