# SQL definition for columns
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what
#          you are doing.

$tb @{name} $sp @{type}

%if @{not-null} %then
 [ NOT NULL]
%end

%if @{default-value} %then
 [ DEFAULT ] @{default-value}
%end
,$br
