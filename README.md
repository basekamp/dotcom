# Basekamp k8s

## Install

```sh
helm upgrade $RELEASE_NAME drupal-0.12.0-probes.tgz -f helm-values.yaml --set mariadb.mariadbPassword=$SEE_LASTPASS --namespace=basekamp
```
