Display a multistep form using native Drupal form API and $form_state manipulation.
You have to write a separate form for each step, and then call na_multistep_form_get() with
an array $steps, saying which form has to be displayed in which step.

You also need to specify a "submit_callback" function, this is where you write code to do
something with all collected values.

Please note that values are not store in $form_state['values'], but are stored separatly for each step
in $form_state['step_informations'][x]['values'] (where x is the index of step).

What this module does NOT do for the moment :
- ajax from one step to another
- get parameter in url to skip steps : all steps must be filled one by one. This is something thay should not be
  too hard to implements, though.

Example :

@code
<?php

function _rf_cv_form_steps() {
  $path = drupal_get_path('module', 'rf_cv');
  return array(
    1 => array(
      'form'  => 'rf_cv_form_identity',
      'title' => t('Identité (1)'),
      'file'  => "$path/rf_cv.step_1.inc",
     ),
   2 => array(
     'form' => 'rf_cv_form_identity_2',
     'title' => t('Identité (2)'),
     'file'  => "$path/rf_cv.step_2.inc",
    ),
  );
}

// display our multistep form in a page callback :
function rf_cv_page() {
  $infos = array('steps' => _rf_cv_form_steps(), 'submit_callback' => 'rf_cv_multistep_form_submit');
  return drupal_render(na_multistep_form_get($infos);
}
@endcode


@param array $infos
  an associative array describing options to construct our form :
  - steps (array, required)
    - form (string, required) will contain the name of the function declaring the form.
    - title (string, required) will be used for page title on this step.
    - file (string, optionnal) : file to include for this step. Use that to put each step in its own file.
  - submit_callback (string, required) : submit callback to execute when ALL steps are done. *

@return array
  like drupal_get_form(), you need then to run drupal_render() to actually display the form.


