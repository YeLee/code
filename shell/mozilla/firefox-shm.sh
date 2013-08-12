[ -d  ~/.mozilla/firefox/.config/extensions ] || sh ~/.mozilla/extension.sh 
sh ~/.mozilla/dlflash.sh
#[ -z "$(pgrep cdnproxy)" ] || export http_proxy="http://localhost:8080"
exec firefox
