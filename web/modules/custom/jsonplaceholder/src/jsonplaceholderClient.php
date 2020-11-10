<?php
namespace Drupal\jsonplaceholder;

use Drupal\Core\Http\ClientFactory;
use Drupal\Component\Serialization\Json;



/**
 * Class JsonplaceholderClient.
 *
 * @package Drupal\jsonplaceholder
 */
class JsonplaceholderClient{
  /**
   * The HTTP client
   * @var \GuzzleHttp\ClientInterface
   */
  protected $client;

  public function __construct(ClientFactory $http_client_factory){
    $this->client=$http_client_factory->fromOptions([
      'base_uri'=>'https://jsonplaceholder.typicode.com',
      'timeout'=>3,
    ]);
  }
  /**
   * Get posts from the jsonplaceholder webpage
   */
  public function getPosts(){

    $response = $this->client->get('posts');
    $body=$response->getBody();

    $items = Json::decode($response->getBody());

    $items=array_slice($items,8,10);

    return $items;


  }


}
