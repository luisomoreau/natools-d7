<?php
/**
 * @file
 * Hooks provided by the na_multistep_form module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter $form_state BEFORE it is sent to step forms. This is where you can change values
 * before displaying the form.
 */
function hook_na_multistep_form_state_alter(&$form_state) {
  $current_step = $form_state['step'];
  $form_state['step_information'][$current_step]['values']['tintin'] = 'milou';

  // you may also create your own variables in form_state : they will be available for all steps forms
  // global $user;
  $form_state['my_user_variable'] = 'hello world';
}

/**
 * @} End of "addtogroup hooks".
 */