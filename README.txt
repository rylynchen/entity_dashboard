INSTALL
==========

1. Extract module into /sites/all/modules/x folder.
2. Enable "Entity Dashboard" on /admin/modules page.


Config
==========

1.Config permission in admin/people/permissions.
2.Config allowed blocks in admin/config/system/entity_dashboard.

Theme
==========
To create a theme used in dashboard, must be add in .css file to define allowed control regions width, like:
#dashboard div#[REGION_NAME] {width:[WIDTH_VALUE]}

Examples:
#dashboard div#content {
  width: 59%;
  outline:0;
}

#dashboard div#sidebar_first {
  width: 20%;
}

#dashboard div#sidebar_second {
  width: 20%;
}