<?php
namespace Drupal\jsonplaceholder\Controller;
use Drupal\Core\Controller\ControllerBase;

/**
 * Class JsonplaceHolderController.
 *
 * @package Drupal\jsonplaceholder\Controller
 */
class JsonplaceHolderController extends ControllerBase{

  /**
   * @return array
   */

  function render()
  {
    $jsonplaceholder_client=\Drupal::service( 'jsonplaceholder.jsonplaceholder_client');
    $items= $jsonplaceholder_client->getPosts();

    return [

      '#type'=>'markup',
      '#markup'=>'',

    ];
  }
}
