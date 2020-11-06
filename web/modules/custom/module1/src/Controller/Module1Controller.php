<?php
namespace Drupal\module1\Controller;

/**
 * Class Module1Controller.
 *
 * @package Drupal\module1\Controller
 */
class Module1Controller{

  /**
   * @return array
   */

  function render()
  {
    return [

      '#theme'=>'post_list',
      '#title'=>'undefined',
      '#post1'=>'uno',
      '#post2'=>'dos',
      '#post3'=>'tres',
      '#post4'=>'cuatro',
      '#items'=>NULL,
    ];
  }
}


