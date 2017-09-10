FROM php:5-apache 

ENV GOTPL_VER 0.1.5
ENV GOTPL_URL https://github.com/wodby/gotpl/releases/download/${GOTPL_VER}/gotpl-linux-amd64-${GOTPL_VER}.tar.gz

RUN apt-get update && apt-get install -y \
    wget \
    zip

# Composer.
RUN curl --silent --output /tmp/composer-setup.php https://getcomposer.org/installer && \
    php /tmp/composer-setup.php --install-dir=/usr/local/bin --filename=composer && \
    rm /tmp/composer-setup.php

# Drush.
RUN composer global require drush/drush:8.1.13 && \
    ln -s /root/.composer/vendor/bin/drush /usr/local/bin/drush

# Go TPL.
RUN wget -qO- ${GOTPL_URL} | tar xz -C /usr/local/bin

ENV DRUPAL_SETTINGS_PATH ${DRUPAL_SETTINGS_PATH:-}
ENV MARIADB_HOST ${MARIADB_HOST:-}
ENV MARIADB_DATABASE ${MARIADB_DATABASE:-}
ENV MARIADB_USER ${MARIADB_USER:-}
ENV MARIADB_PASSWORD ${MARIADB_PASSWORD:-}
COPY templates /etc/gotpl/

WORKDIR /var/www/html

COPY html .
RUN mkdir sites/default/files && chmod a+w sites/default/files && \
    gotpl /etc/gotpl/settings.php.tpl > sites/default/settings.php && \
    chown -R $APACHE_RUN_USER:$APACHE_RUN_GROUP /var/www/html

EXPOSE 80
CMD ["apache2-foreground"]
