<?php

class omniture_reaction_variables extends context_reaction {
  /**
   * Implements context_reaction::options_form()
   *
   * Allow users to specify varables that should be set for the given context.
   */
  function options_form($context) {
    $settings = $this->fetch_from_context($context);

    // Get the existing variables from context.
    $existing_variables = isset($settings['omniture_variables']) ? $settings['omniture_variables'] : array();

    // Build the variables form.
    _omniture_variables_form($form, $existing_variables);

    // Modify the #parents property to make sure values are stored in the
    // location that contexts expects them.
    $parents = array('reactions', 'plugins', $this->plugin);

    foreach (element_children($form['omniture_variables']) as $key) {
      if (is_numeric($key)) {
        $form['omniture_variables'][$key]['name']['#parents'] = array_merge($parents, $form['omniture_variables'][$key]['name']['#parents']);
        $form['omniture_variables'][$key]['value']['#parents'] = array_merge($parents, $form['omniture_variables'][$key]['value']['#parents']);
      }
    }

    // Modify form to call the contexts-specific versions of certain functions.
    $form['add_another_variable']['#submit'] = array('omniture_context_add_another_variable_submit');
    $form['add_another_variable']['#ajax']['callback'] = 'omniture_context_add_another_variable_js';

    return $form;
  }

  /**
   * Implements context_reaction::execute()
   *
   * Updates the $variables array based on the current context. Note that tokens
   * will be replaced later in _omniture_format_variables().
   *
   * @param array $variables
   *   The variables array, passed by reference. Context defined variables will
   *   be added as new keys in this array.
   */
  function execute(&$variables) {
    foreach ($this->get_contexts() as $context) {
      foreach ($context->reactions['omniture_vars']['omniture_variables'] as $key => $value) {
        $variables[$value['name']] = $value['value'];
      }
    }
  }
}

/**
 * Submit handler to add more variables in the context reaction form.
 */
function omniture_context_add_another_variable_submit($form, &$form_state) {
  $form_state['omniture_variables'] = $form_state['input']['reactions']['plugins']['omniture_vars'];
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for adding more variables in the context reaction form.
 */
function omniture_context_add_another_variable_js($form, $form_state) {
  return $form['reactions']['plugins']['omniture_vars']['omniture_variables'];
}
